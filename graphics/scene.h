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

#include "../external/imgui/imgui.h"

#define FPS_FRAME_UPDATE_RATE 50
#define TIME_NORMALIZING_FACTOR 1.0f

class Scene {
public:
    Scene(Resources& resources);
    virtual ~Scene();

    void addForces(const std::shared_ptr<Force>& f);

    void drawMenu();
    void render(glm::mat4& view, glm::mat4& projPersp);

    std::vector<std::shared_ptr<Force>> forces;

private:
    void applyCollisionPhysics();
    void handlePausePlay() {
        if (this->state.pause && ImGui::Button("Play")) this->state.pause = false;
        else if (!this->state.pause && ImGui::Button("Pause")) this->state.pause = true;
    }
    void handleTimeSpeed() {
        if (ImGui::Button("0.25x")) this->state.timeMultiplier = 0.25f;
        ImGui::SameLine();
        if (ImGui::Button("0.5x")) this->state.timeMultiplier = 0.5f;
        ImGui::SameLine();
        if (ImGui::Button("1x")) this->state.timeMultiplier = 1.0f;
        ImGui::SameLine();
        if (ImGui::Button("2x")) this->state.timeMultiplier = 2.0f;
        ImGui::SameLine();
        if (ImGui::Button("4x")) this->state.timeMultiplier = 4.0f;
    }

    void resetToOriginalState() {
        if (ImGui::Button("Reset Scene")) {
            this->m_res.reset();
            this->state.timeElapsed = 0.0f;
            this->state.timeMultiplier = 1.0f;
        }
    }

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
    struct SceneState {
        bool pause = true;
        float timeMultiplier = 1.0f;    // 1X
        double timeElapsed = 0.0f;
    };

    SceneState state;
};

#endif //PHYSICS_ENGINE_SCENE_H
