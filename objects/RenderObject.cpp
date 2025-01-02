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

void RenderObject::setContextFromScene(const std::shared_ptr<SceneContext>& sceneContext) {
    this->context = sceneContext;
}

void RenderObject::renderCollisionMesh(glm::mat4 &view, glm::mat4 &projPersp, const PhysicsState& oldState) {
    if (this->context->renderCollisionMesh) {
        auto differences = PhysicsState::getDifferenceInStates(oldState, *this->physics);
        const std::vector<float> position = {
                this->physics->getPositionOfCM()[0], this->physics->getPositionOfCM()[1],this->physics->getPositionOfCM()[2],
        };
        this->collisionMesh->translate(position);
        this->collisionMesh->rotate(differences.second);
        this->collisionMesh->render(view, projPersp);
    }
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

void RenderObject::reset() {
    this->physics->reset();
    this->translateCollisionMeshToState();
    this->rotateCollisionMeshToState();
}

void RenderObject::resize(const float &x, const float &y, const float &z) { /* Do Nothing */ }

void RenderObject::translateCollisionMeshToState() {
    const std::vector<float> translation= {
            this->physics->getPositionOfCM()[0],
            this->physics->getPositionOfCM()[1],
            this->physics->getPositionOfCM()[2]
    };
    this->collisionMesh->translate(translation);
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
        for (auto && object : o->getObjects()) {
            if (thisObject->isIntersecting(object)) thisObject->physics->applyCollision(object->physics);
        }
    }
}

std::vector<std::shared_ptr<PhysicsState>> RenderObject::getPhysicState() {
    return std::vector<std::shared_ptr<PhysicsState>>({ this->physics });
}
