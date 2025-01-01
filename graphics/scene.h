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
#include <math.h>

#include "./Resources.h"
#include "../physics/forces.h"
#include "./sceneContext.h"

#include "./public-libs/imgui/imgui.h"

#define FPS_FRAME_UPDATE_RATE 50

class Scene {
public:
    Scene(Resources& resources);
    virtual ~Scene();

    void addForces(const std::shared_ptr<Force>& f);

    void drawMenu();
    void render(glm::mat4& view, glm::mat4& projPersp);

    std::vector<std::shared_ptr<Force>> forces;

protected:
    Resources& m_res;
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    u_int8_t tick;
    float fps;
    int lastFps;
    TimePoint startTime;
    std::shared_ptr<SceneContext> context;
    void applyCollisionPhysics();
};

#endif //PHYSICS_ENGINE_SCENE_H
