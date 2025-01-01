//
// Created by Emir Tuncbilek on 12/21/24.
//

#include <random>

#include "CloudParticuleObject.h"

inline float randomFloatInRange(float minVal, float maxVal) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(minVal, maxVal);
    return dist(gen);
}

CloudParticuleObject::CloudParticuleObject(const CloudParticuleObject &other) : RenderObject(other) {
    this->_numParticules = other._numParticules;
    this->_maxXDistance = other._maxXDistance;
    this->_maxYDistance = other._maxYDistance;
    this->_airSpeed = other._airSpeed;
    this->particuleObjectPath = other.particuleObjectPath;
    this->particules.clear();
    this->particules.reserve(this->_numParticules);
    for (auto && otherParticule : other.particules) {
        // std::shared_ptr<ParticuleObject> clone = otherParticule->clone();
        this->particules.push_back(otherParticule->clone());
    }
}

void CloudParticuleObject::init() {
    size_t totalParticles = this->_numParticules;
    size_t updateInterval = totalParticles / 100; // Update every 1% of particules loaded
    float totalParticuleSpeed = 0.0f;
    for (size_t i = 0; i < totalParticles; i++) {
        std::shared_ptr<ParticuleObject> o = std::make_shared<ParticuleObject>(
                this->particuleObjectPath,
                randomFloatInRange(-this->_maxXDistance, this->_maxXDistance),
                randomFloatInRange(-this->_maxYDistance, this->_maxYDistance)
        );
        o->shaderPaths = this->shaderPaths;
        o->speed = glm::vec3(this->_airSpeed, 0.0f, 0.0f);
        totalParticuleSpeed += o->speed.x;
        o->init();
        this->particules.push_back(std::move(o));

        if (i % updateInterval == 0 || i == totalParticles - 1) {
            float progress = (float)(i + 1) / (float) totalParticles * 100.0f;
            std::cout << "Loading: " << progress << "%\r" << std::flush;
        }
    }
    const std::vector<float> zeros = { ZERO_VECTOR };
    const std::vector<float> pos = { -5.0f, 1.0f, 0.0f };
    const std::vector<float> velocity = { totalParticuleSpeed/(float)totalParticles, 0.0f, 0.0f };         // the velocity of the cloud is simply the average speed of all the particules
    this->physics = std::make_shared<PhysicsState>(0.0f, 1.0f, 0.0f, pos, zeros, velocity, zeros);  // helps picture the cloud of particules as a single rigid body
    std::cout << "Loading: 100% complete!" << std::endl;
}

std::vector<std::shared_ptr<RenderObject>> CloudParticuleObject::getObjects() const {
    std::vector<std::shared_ptr<RenderObject>> objects;
    objects.reserve(this->_numParticules);
    for (auto && particule : this->particules) { objects.push_back(particule->clone()); }
    return objects;
}

void CloudParticuleObject::setContextFromScene(const std::shared_ptr<SceneContext> &sceneContext) {
    RenderObject::setContextFromScene(sceneContext);
    for (auto && object : this->particules) object->setContextFromScene(sceneContext);
}

void CloudParticuleObject::render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) {
    this->physics->updateTimeDelta(delta_t);
    this->physics->nextFrame();
    for (size_t i = 0; i < this->_numParticules; i ++) {
        this->particules[i]->render(view, projPersp, delta_t);
    }
}

std::vector<std::shared_ptr<PhysicsState>> CloudParticuleObject::getPhysicState() {
    std::vector<std::shared_ptr<PhysicsState>> vec;
    vec.reserve(this->_numParticules);
    for (int i = 0; i < this->_numParticules; i ++) {
        vec.push_back(this->particules[i]->physics);
    }
    return vec;
}

std::shared_ptr<RenderObject> CloudParticuleObject::clone() const {
    return std::make_unique<CloudParticuleObject>(*this);
}
