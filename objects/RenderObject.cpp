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
    this->modelTexture = nullptr;
    this->shaderPaths = {};
    this->mvpModelLocation = -1;
    this->physics = nullptr;
}

RenderObject::RenderObject(const std::string& modelPath) {
    this->model3D = std::make_shared<Model>(modelPath.c_str());
    this->modelShaderProgram = std::make_shared<ShaderProgram>();
    this->modelTexture = nullptr;
    this->shaderPaths = {};
    this->mvpModelLocation = -1;
    this->physics = nullptr;
}

RenderObject::RenderObject(const RenderObject& other) {
    this->model3D = other.model3D;
    this->modelShaderProgram = other.modelShaderProgram;
    this->modelTexture = other.modelTexture;
    this->mvpModelLocation = other.mvpModelLocation;
    this->shaderPaths = other.shaderPaths;
    this->physics = other.physics;
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

std::vector<std::shared_ptr<PhysicsState>> RenderObject::getPhysicState() {
    std::vector<std::shared_ptr<PhysicsState>> vec;
    vec.reserve(1);
    vec.push_back(this->physics);
    return vec;
}

void RenderObject::computeCollisions(std::unique_ptr<RenderObject> &o) {
    for (auto & physicsState : this->getPhysicState()) {
        float * CM1_ptr = physicsState->getPositionOfCM();
        float CM1[3];
        std::copy(CM1_ptr, CM1_ptr + DIMENSIONS, CM1);

        for (auto & otherState : o->getPhysicState()) {
            // preliminary check to see if an object is potentially in collision
            float * CM2_ptr = otherState->getPositionOfCM();
            float CM2[3];
            std::copy(CM2_ptr, CM2_ptr + DIMENSIONS, CM2);
            float distanceBetweenCMsSquared = (CM2[0] - CM1[0]) * (CM2[0] - CM1[0]) +
                                              (CM2[1] - CM1[1]) * (CM2[1] - CM1[1]) +
                                              (CM2[2] - CM1[2]) * (CM2[2] - CM1[2]);
            float minPhysicalDistanceSquared = (physicsState->maximumRadius + otherState->maximumRadius);
            if (distanceBetweenCMsSquared <= minPhysicalDistanceSquared) {
                physicsState->applyCollision(otherState);
            }
        }
    }
}
