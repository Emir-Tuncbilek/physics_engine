//
// Created by Emir Tuncbilek on 12/26/24.
//

#include "BoundingBox.h"

BoundingBox::BoundingBox(const std::vector<float>& center, const float &width, const float &depth, const float &height) :
    BoundingVolume(center), width(width), depth(depth), height(height) { }

bool BoundingBox::isCollidingWith(std::shared_ptr<BoundingVolume> boundingVolume) {
    std::shared_ptr<BoundingBox> object = std::make_shared<BoundingBox>(*this);
    return this->testNormals(boundingVolume) || boundingVolume->testNormals(object);
}

std::shared_ptr<BoundingVolume> BoundingBox::clone() {
    auto box = std::make_shared<BoundingBox>(*this);
    return box;
}

bool BoundingBox::testNormals(std::shared_ptr<BoundingVolume> boundingVolume) {
    // predetermine the normals to save in computations (no need to cross product and normalize)
    const std::vector<std::vector<float>> normals = {
            {  1.0f,  0.0f,  0.0f }, // +X
            {  0.0f,  1.0f,  0.0f }, // +Y
            {  0.0f,  0.0f,  1.0f }, // +Z
            { -1.0f,  0.0f,  0.0f }, // -X
            {  0.0f, -1.0f,  0.0f }, // -Y
            {  0.0f,  0.0f, -1.0f }  // -Z
    };

    for (uint8_t i = 0; i < NUM_NORMALS; i ++) {
        // reorient the normal to the real orientation
        std::vector<float> normal;
        this->applyRotationMatrix(normals[i], normal);
        // detect collision
        float objectMin = this->getMin(normal),           objectMax = this->getMax(normal);
        float otherMin  = boundingVolume->getMin(normal), otherMax = boundingVolume->getMax(normal);
        if (!SATIsCollision(objectMin, objectMax, otherMin, otherMax)) return false;
    }
    return true;
}

float BoundingBox::getMin(const std::vector<float>& axis) const {
    return this->getPositionOf(axis, [](float a, float b) { return a < b; });
}

float BoundingBox::getMax(const std::vector<float>& axis) const  {
    return this->getPositionOf(axis, [](float a, float b) { return a < b; });
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
            vertices[0][0] + this->center[0],
            vertices[0][1] + this->center[1],
            vertices[0][2] + this->center[2]
    };
    float projection = dotProduct(vertex, axis, DIMENSIONS);
    for (uint8_t i = 1; i < NUM_VERTICES; i ++) {
        this->applyRotationMatrix(vertex, vertex);
        vertex[0] = vertices[i][0] + this->center[0];
        vertex[1] = vertices[i][1] + this->center[1];
        vertex[2] = vertices[i][2] + this->center[2];
        float currentProjection = dotProduct(vertex, axis, DIMENSIONS);
        if (comparator(currentProjection, projection)) projection = currentProjection;
    }
    return projection;
}