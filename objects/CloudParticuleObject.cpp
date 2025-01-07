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
    this->particuleObjectPath = other.particuleObjectPath;
    this->particules.clear();
    this->particules.reserve(this->_numParticules);
    this->height = other.height;
    this->width = other.width;
    this->depth = other.depth;
    for (auto && otherParticule : other.particules) {
        this->particules.push_back(otherParticule->clone());
    }
}

void CloudParticuleObject::init(const std::shared_ptr<PhysicsState>& p) {
    size_t totalParticles = this->_numParticules;
    size_t updateInterval = totalParticles / 100; // Update every 1% of particules loaded
    for (size_t i = 0; i < totalParticles; i++) {
        std::shared_ptr<ParticuleObject> o = std::make_shared<ParticuleObject>(this->particuleObjectPath);
        o->shaderPaths = this->shaderPaths;
        auto particulePhysics = *p;
        particulePhysics.setNewPos({
            p->getPositionOfCM()[0] + randomFloatInRange(this->width.first,  this->width.second),
            p->getPositionOfCM()[1] + randomFloatInRange(this->depth.first,  this->depth.second),
            p->getPositionOfCM()[2] + randomFloatInRange(this->height.first, this->height.second)
        });
        o->init(std::make_shared<PhysicsState>(particulePhysics));
        this->particules.push_back(std::move(o));

        if (i % updateInterval == 0 || i == totalParticles - 1) {
            float progress = (float)(i + 1) / (float) totalParticles * 100.0f;
            std::cout << "Loading: " << progress << "%\r" << std::flush;
        }
    }

    RenderObject::init(p);
    std::cout << "Loading: 100% complete!" << std::endl;
}

void CloudParticuleObject::reset() {
    for (auto && particule : this->particules) particule->reset();
}

std::vector<std::shared_ptr<RenderObject>> CloudParticuleObject::getObjects() {
    std::vector<std::shared_ptr<RenderObject>> objects;
    objects.reserve(this->_numParticules);
    for (auto && particule : this->particules) { objects.push_back(particule->shared_from_this()); }
    return objects;
}

void CloudParticuleObject::setContextFromScene(const std::shared_ptr<SceneContext> &sceneContext) {
    RenderObject::setContextFromScene(sceneContext);
    for (auto && object : this->particules) object->setContextFromScene(sceneContext);
}

void CloudParticuleObject::render(glm::mat4 &view, glm::mat4 &projPersp) {
    for (size_t i = 0; i < this->_numParticules; i ++) {
        this->particules[i]->render(view, projPersp);
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

void CloudParticuleObject::updatePhysics(const float &delta_t) {
    RenderObject::updatePhysics(delta_t);
    for (auto && particule : this->particules) particule->updatePhysics(delta_t);
}
