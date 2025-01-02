//
// Created by Emir Tuncbilek on 8/29/24.
//

#include "scene.h"

Scene::Scene(Resources &resources) : m_res(resources) {
    this->startTime = Clock::now();
    this->tick = 0;
    this->fps  = 0.0f;
    this->lastFps = 0;
    this->context = std::make_shared<SceneContext>();
    this->context->renderCollisionMesh = false;
    for (auto && cluster : this->m_res.objects) cluster->setContextFromScene(this->context);
    this->state = SceneState();
}

Scene::~Scene() = default;



void Scene::addForces(const std::shared_ptr<Force> &f) {
    this->forces.push_back(f);
}

void Scene::drawMenu() {
    ImGui::Begin("Menu");
    ImGui::Checkbox("Display Collision Mesh", (bool *) &this->context->renderCollisionMesh);
    ImGui::Text("Time Elapsed: %.3f secs", this->state.timeElapsed);

    this->handlePausePlay();
    ImGui::Text("Speed: %.2fx", this->state.timeMultiplier);
    this->handleTimeSpeed();

    this->resetToOriginalState();

    ImGui::Text("FPS: %d Hz", this->tick >= FPS_FRAME_UPDATE_RATE ? (int) round(this->fps) : this->lastFps);

    if (this->tick >= FPS_FRAME_UPDATE_RATE) {
        this->tick = 0;
        this->lastFps = (int) round(this->fps);
    }

    ImGui::End();
}

void Scene::render(glm::mat4 &view, glm::mat4 &projPersp) {

    auto endTime = Clock::now();
    float delta_t = std::chrono::duration<float>(endTime - this->startTime).count();
    this->fps = 1.0f / delta_t;
    delta_t *= this->state.pause ? 0.0f : this->state.timeMultiplier;
    this->state.timeElapsed += (double) delta_t;
    delta_t *= TIME_NORMALIZING_FACTOR;
    this->startTime = endTime;

    this->drawMenu();

    for (int i = 0; i < this->m_res.objects.size(); i ++) {
        for (int j = 0; j < this->forces.size(); j ++ ) {
            std::vector<std::shared_ptr<PhysicsState>> states = this->m_res.objects[i]->getPhysicState();
            this->forces[j]->apply(states);
            for (std::shared_ptr<PhysicsState>& state : states) state->resetAccelerations();
        }
        this->m_res.objects[i]->render(view, projPersp, delta_t);
    }
    this->applyCollisionPhysics();
    this->tick ++;
}


void Scene::applyCollisionPhysics() {
    for (int i = 0; i < this->m_res.objects.size(); i ++) {
        for (int j = i + 1; j < this->m_res.objects.size(); j++) {
            this->m_res.objects[i]->computeCollisions(this->m_res.objects[j]);
            this->m_res.objects[i]->physics->nextFrame();
        }
    }
}