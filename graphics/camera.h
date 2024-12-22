//
// Created by Emir Tuncbilek on 8/29/24.
//

#ifndef PHYSICS_ENGINE_CAMERA_H
#define PHYSICS_ENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./window.h"

#define CAMERA_TURN_RATE 0.01f

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec2& orientation);
    ~Camera();

    glm::mat4 getViewMatrix();

    void update(Window& w, const float& speed);

private:
    glm::vec3 m_position;
    glm::vec2 m_orientation;
};

#endif //PHYSICS_ENGINE_CAMERA_H
