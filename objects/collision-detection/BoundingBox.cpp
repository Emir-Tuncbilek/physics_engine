//
// Created by Emir Tuncbilek on 12/26/24.
//

#include "BoundingBox.h"
#include "../CollisionObject.h"

BoundingBox::BoundingBox(const float &width, const float &depth, const float &height) :
    BoundingVolume(), width(width), depth(depth), height(height) {
    this->renderObject = std::make_shared<CollisionObject>(BOX_OBJECT_FILE_PATH);
    this->renderObject->shaderPaths.emplace_back(BOUNDING_VOLUME_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
    this->renderObject->shaderPaths.emplace_back(BOUNDING_VOLUME_GEOM_SHADER_PATH, GL_GEOMETRY_SHADER);
    this->renderObject->shaderPaths.emplace_back(BOUNDING_VOLUME_FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER);
    this->renderObject->init(this->parentPhysics);
}

std::pair<bool, std::vector<float>> BoundingBox::isCollidingWith(std::shared_ptr<BoundingVolume> boundingVolume) {
    auto object = this->shared_from_this();
    auto resultThis = this->testNormals(boundingVolume);
    if (resultThis.first) return resultThis;
    auto resultOther = boundingVolume->testNormals(object);
    if (resultOther.first) return { true, {-resultOther.second[0], -resultOther.second[1], -resultOther.second[2]} };
    return { false, {} };
}

std::shared_ptr<BoundingVolume> BoundingBox::clone() {
    return std::make_shared<BoundingBox>(*this);
}

std::pair<bool, std::vector<float>> BoundingBox::testNormals(std::shared_ptr<BoundingVolume> boundingVolume) {
    // predetermine the normals to save in computations (no need to cross product and normalize)
    const std::vector<std::vector<float>> normals = {
            {  1.0f,  0.0f,  0.0f }, // +X
            {  0.0f,  1.0f,  0.0f }, // +Y
            {  0.0f,  0.0f,  1.0f }, // +Z
            { -1.0f,  0.0f,  0.0f }, // -X
            {  0.0f, -1.0f,  0.0f }, // -Y
            {  0.0f,  0.0f, -1.0f }  // -Z
    };
    std::vector<float> normal;
    for (uint8_t i = 0; i < NUM_NORMALS; i ++) {
        // reorient the normal to the real orientation
        // this->applyRotationMatrix(normals[i], normal);
        // detect collision
        float objectMin = this->getMin(normals[i]),           objectMax = this->getMax(normals[i]);
        float otherMin  = boundingVolume->getMin(normals[i]), otherMax = boundingVolume->getMax(normals[i]);
        if (!SATIsCollision(objectMin, objectMax, otherMin, otherMax)) {
            return { false, {} };
        }
    }
    std::vector<float> collisionNormal;
    for (uint8_t i = 0; i < DIMENSIONS; i ++) {
        collisionNormal.push_back(boundingVolume->parentPhysics->getPositionOfCM()[i] - this->parentPhysics->getPositionOfCM()[i]);
    }
    normalize(collisionNormal);
    return { true, BoundingBox::findBestNormal(collisionNormal) };
}

float BoundingBox::getMin(const std::vector<float>& axis) const {
    return this->getPositionOf(axis, [](float a, float b) { return a < b; });
}

float BoundingBox::getMax(const std::vector<float>& axis) const  {
    return this->getPositionOf(axis, [](float a, float b) { return a > b; });
}

float BoundingBox::getPositionOf(const std::vector<float>& axis, const std::function<bool(float, float)> &comparator) const {
    float vertices[NUM_VERTICES][DIMENSIONS] = {
            {  width * 0.5f,  depth * 0.5f,  height * 0.5f },
            {  width * 0.5f,  depth * 0.5f, -height * 0.5f },
            {  width * 0.5f, -depth * 0.5f,  height * 0.5f },
            {  width * 0.5f, -depth * 0.5f, -height * 0.5f },
            { -width * 0.5f,  depth * 0.5f,  height * 0.5f },
            { -width * 0.5f,  depth * 0.5f, -height * 0.5f },
            { -width * 0.5f, -depth * 0.5f,  height * 0.5f },
            { -width * 0.5f, -depth * 0.5f, -height * 0.5f }
    };
    std::vector<float> vertex = {
            vertices[0][0] + this->parentPhysics->getPositionOfCM()[0],
            vertices[0][1] + this->parentPhysics->getPositionOfCM()[1],
            vertices[0][2] + this->parentPhysics->getPositionOfCM()[2]
    };
    float projection = dotProduct(vertex, axis, DIMENSIONS);
    for (uint8_t i = 1; i < NUM_VERTICES; i ++) {
        vertex[0] = vertices[i][0] + this->parentPhysics->getPositionOfCM()[0];
        vertex[1] = vertices[i][1] + this->parentPhysics->getPositionOfCM()[1];
        vertex[2] = vertices[i][2] + this->parentPhysics->getPositionOfCM()[2];
        float currentProjection = dotProduct(vertex, axis, DIMENSIONS);
        if (comparator(currentProjection, projection)) projection = currentProjection;
    }
    return projection;
}

void BoundingBox::render(glm::mat4 &view, glm::mat4 &projPersp) const {
    this->renderObject->render(view, projPersp);
}

std::vector<float> BoundingBox::findBestNormal(const std::vector<float>& normal) {
    const std::vector<std::vector<float>> normals = {
            {  1.0f,  0.0f,  0.0f }, // +X
            {  0.0f,  1.0f,  0.0f }, // +Y
            {  0.0f,  0.0f,  1.0f }, // +Z
            { -1.0f,  0.0f,  0.0f }, // -X
            {  0.0f, -1.0f,  0.0f }, // -Y
            {  0.0f,  0.0f, -1.0f }  // -Z
    };
    float loss;
    float bestLoss = std::numeric_limits<float>::infinity();
    std::vector<float> bestNormal;
    for (uint8_t i = 0; i < NUM_NORMALS; i ++) {
        loss = 0.0f;
        for (uint8_t j = 0; j < DIMENSIONS; j ++) loss += (normals[i][j] - normal[j]) * (normals[i][j] - normal[j]);
        if (loss < bestLoss) {
            bestLoss = loss;
            bestNormal = normals[i];
        }
    }
    return bestNormal;
}
