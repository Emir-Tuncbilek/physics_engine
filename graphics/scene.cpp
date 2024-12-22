//
// Created by Emir Tuncbilek on 8/29/24.
//

#include "scene.h"

Scene::Scene(Resources &resources) : m_res(resources) {
    this->startTime = std::clock();
}

Scene::~Scene() = default;

void Scene::addForces(const std::shared_ptr<Force> &f) {
    this->forces.push_back(f);
}

void Scene::render(glm::mat4 &view, glm::mat4 &projPersp) {
    auto endTime = std::clock();
    float delta_t = (float)(endTime - this->startTime) / CLOCKS_PER_SEC;
    this->startTime = endTime;
    for (int i = 0; i < this->m_res.objects.size(); i ++) {
        for (int j = 0; j < this->forces.size(); j ++ ) {
            std::vector<std::shared_ptr<PhysicsState>> states = this->m_res.objects[i]->getPhysicState();
            this->forces[j]->apply(states);
            for (std::shared_ptr<PhysicsState>& state : states) state->resetAccelerations();
        }
        this->m_res.objects[i]->render(view, projPersp, delta_t);
    }
    this->applyCollisionPhysics();
}

void Scene::applyCollisionPhysics() {
    for (int i = 0; i < this->m_res.objects.size(); i ++) {
        for (int j = i + 1; j < this->m_res.objects.size(); j++) {
            this->m_res.objects[i]->computeCollisions(this->m_res.objects[j]);
            this->m_res.objects[i]->physics->nextFrame();
        }
    }
}