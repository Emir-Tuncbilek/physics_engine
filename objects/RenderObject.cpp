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
    this->lastComputedNormal = {};
    this->renderOffset = {};
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
    this->collidingWith = nullptr;
    this->lastComputedNormal = {};
    this->renderOffset = {};
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
    this->collidingWith = other.collidingWith;
    this->lastComputedNormal = other.lastComputedNormal;
    this->renderOffset = other.renderOffset;
}

void RenderObject::setContextFromScene(const std::shared_ptr<SceneContext>& sceneContext) {
    this->context = sceneContext;
}

void RenderObject::renderCollisionMesh(glm::mat4 &view, glm::mat4 &projPersp, const PhysicsState& oldState) {
    if (this->context->renderCollisionMesh) {
        glDisable(GL_CULL_FACE);
        this->collisionMesh->render(view, projPersp);
        glEnable(GL_CULL_FACE);
    }
}

void RenderObject::init(const std::shared_ptr<PhysicsState>& p) {
    for (const auto& path : this->shaderPaths) {
        std::string shaderCode = readFile(path.first);
        Shader s(path.second, shaderCode.c_str());
        this->modelShaderProgram->attachShader(s);
    }
    this->physics = p;
    this->renderOffset = {};
    this->collisionMesh->parentPhysics = this->physics;
    this->modelShaderProgram->link();
    this->mvpModelLocation = this->modelShaderProgram->getUniformLoc("U_MVP");
    if (this->mvpModelLocation == -1) {
        std::cerr << "\"U_MVP not found in the vertex shader file\"" << std::endl;
        exit(1);
    }
}

void RenderObject::reset() {
    this->physics->reset();
}

void RenderObject::resize(const float &x, const float &y, const float &z) { }

void RenderObject::collisionMeshScale(const float &x, const float &y, const float &z) {
    this->collisionMesh->resize(x, y, z);
}

void RenderObject::nudgeCollisionMesh(const std::vector<float> &offset) {
    this->collisionMesh->nudgeCollisionMesh(offset);
}

void RenderObject::addBoundingVolume(const std::shared_ptr<BoundingVolume> &volume) {
    this->collisionMesh->addBoundingVolume(volume);
}

void RenderObject::computeCollisions(std::shared_ptr<RenderObject> &o) {
    for (auto && thisObject : this->getObjects()) {
        for (auto && object : o->getObjects()) {
            auto collisionPair = thisObject->collisionMesh->checkCollision(* object->collisionMesh);
            if (collisionPair.first) {
                if (thisObject->physics->isImmobile(*object->physics)) {
                    thisObject->collidingWith = nullptr;
                    object->collidingWith = nullptr;
                }
                if (object == thisObject->collidingWith) continue;
                thisObject->lastComputedNormal = collisionPair.second;
                object->lastComputedNormal = collisionPair.second;
                thisObject->physics->applyCollision(object->physics, collisionPair.second);
                thisObject->collidingWith = object->shared_from_this();
                object->collidingWith = this->shared_from_this();
            } else {
                if (object == thisObject->collidingWith) {
                    thisObject->collidingWith = nullptr;
                    object->collidingWith = nullptr;
                }
            }
        }
    }
}

std::vector<std::shared_ptr<PhysicsState>> RenderObject::getPhysicState() {
    return std::vector<std::shared_ptr<PhysicsState>>({ this->physics });
}

void RenderObject::updatePhysics(const float &delta_t) {
    this->physics->updateTimeDelta(delta_t);
    this->physics->nextFrame();
}
