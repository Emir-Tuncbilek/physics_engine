//
// Created by Emir Tuncbilek on 8/29/24.
//

#ifndef PHYSICS_ENGINE_SCENE_H
#define PHYSICS_ENGINE_SCENE_H

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <vector>
#include <memory>

#include "./Resources.h"
#include "../physics/forces.h"

class Scene {
public:
    Scene(Resources& resources);
    virtual ~Scene();

    void addForces(const std::shared_ptr<Force>& f);
    void render(glm::mat4& view, glm::mat4& projPersp);

    std::vector<std::shared_ptr<Force>> forces;

protected:
    Resources& m_res;
private:
    std::clock_t startTime;
    void applyCollisionPhysics();
};

#endif //PHYSICS_ENGINE_SCENE_H
