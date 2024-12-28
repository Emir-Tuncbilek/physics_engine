//
// Created by Emir Tuncbilek on 9/6/24.
//

#include "RenderObject.h"


inline std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath.c_str());
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

RenderObject::RenderObject() {
    this->model3D = nullptr;
    this->modelShaderProgram = std::make_shared<ShaderProgram>();
    this->transformShaderProgram = nullptr;
    this->modelTexture = nullptr;
    this->shaderPaths = {};
    this->mvpModelLocation = -1;
    this->physics = nullptr;
    this->collisionMesh = std::make_unique<CollisionMesh>();
}

RenderObject::RenderObject(const std::string& modelPath) {
    this->model3D = std::make_shared<Model>(modelPath.c_str());
    this->modelShaderProgram = std::make_shared<ShaderProgram>();
    this->transformShaderProgram = nullptr;
    this->modelTexture = nullptr;
    this->shaderPaths = {};
    this->mvpModelLocation = -1;
    this->physics = nullptr;
    this->collisionMesh = std::make_unique<CollisionMesh>();
}

RenderObject::RenderObject(const RenderObject& other) {
    this->model3D = other.model3D;
    this->modelShaderProgram = other.modelShaderProgram;
    this->transformShaderProgram = other.transformShaderProgram;
    this->modelTexture = other.modelTexture;
    this->mvpModelLocation = other.mvpModelLocation;
    this->shaderPaths = other.shaderPaths;
    this->physics = other.physics;
    this->collisionMesh = other.collisionMesh->clone();
}

void RenderObject::init() {
    for (const auto& path : this->shaderPaths) {
        std::string shaderCode = readFile(path.first);
        Shader s(path.second, shaderCode.c_str());
        this->modelShaderProgram->attachShader(s);
    }
    this->modelShaderProgram->link();
    this->mvpModelLocation = this->modelShaderProgram->getUniformLoc("U_MVP");
    if (this->mvpModelLocation == -1) {
        std::cerr << "\"U_MVP not found in the vertex shader file\"" << std::endl;
        exit(1);
    }
}

void RenderObject::translateCollisionMeshToState() {
    const std::vector<std::vector<float>> translationMatrix = {
            { 1.0f, 0.0f, 0.0f, this->physics->getPositionOfCM()[0]},
            { 0.0f, 1.0f, 0.0f, this->physics->getPositionOfCM()[1]},
            { 0.0f, 0.0f, 1.0f, this->physics->getPositionOfCM()[2]},
            { 0.0f, 0.0f, 0.0f,              1.0f                  }
    };
    this->collisionMesh->translate(translationMatrix);
}

void RenderObject::rotateCollisionMeshToState() {
    const std::vector<std::vector<float>> rotationMatrix = {
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f },
    };
    this->collisionMesh->rotate(rotationMatrix);
}

void RenderObject::addBoundingVolume(const std::shared_ptr<BoundingVolume> &volume) {
    this->collisionMesh->addBoundingVolume(volume);
}

bool RenderObject::isIntersecting(const std::shared_ptr<RenderObject> &o) const {
    return this->collisionMesh->checkCollision(* o->collisionMesh);
}

void RenderObject::computeCollisions(std::shared_ptr<RenderObject> &o) {
    // data available from this->model3D->m_shape.vertexData;

    for (auto && thisObject : this->getObjects()) {
        float * CM1_ptr = thisObject->physics->getPositionOfCM();
        float CM1[3];
        std::copy(CM1_ptr, CM1_ptr + DIMENSIONS, CM1);

        for (auto && object : o->getObjects()) {
            // preliminary check to see if an object is potentially in collision
            float * CM2_ptr = object->physics->getPositionOfCM();
            float CM2[3];
            std::copy(CM2_ptr, CM2_ptr + DIMENSIONS, CM2);
            float distanceBetweenCMsSquared = (CM2[0] - CM1[0]) * (CM2[0] - CM1[0]) +
                                              (CM2[1] - CM1[1]) * (CM2[1] - CM1[1]) +
                                              (CM2[2] - CM1[2]) * (CM2[2] - CM1[2]);
            float minPhysicalDistanceSquared = (thisObject->physics->maximumRadius + object->physics->maximumRadius);
            if (distanceBetweenCMsSquared <= minPhysicalDistanceSquared && thisObject->isIntersecting(object)) {
                thisObject->physics->applyCollision(object->physics);
            }
        }
    }
}

std::vector<std::shared_ptr<PhysicsState>> RenderObject::getPhysicState() {
    return std::vector<std::shared_ptr<PhysicsState>>({ this->physics });
}
