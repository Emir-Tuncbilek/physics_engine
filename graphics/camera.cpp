//
// Created by Emir Tuncbilek on 8/29/24.
//

#include "camera.h"

Camera::Camera(const glm::vec3 &position, const glm::vec2 &orientation) :
                m_position(position),
                m_orientation(orientation) {}

Camera::~Camera() = default;

glm::mat4 Camera::getViewMatrix() {
    auto view = glm::mat4(1.0f);
    view = glm::rotate(view, -m_orientation.x, glm::vec3(1.0, 0.0, 0.0));
    view = glm::rotate(view, -m_orientation.y, glm::vec3(0.0, 1.0, 0.0));
    view = glm::translate(view, -this->m_position);
    return view;
}

void Camera::update(Window &w, const float& speed) {
    int x, y;
    w.getMouseMotion(x, y);

    this->m_orientation.y -= (float)x * CAMERA_TURN_RATE;
    this->m_orientation.x -= (float)y * CAMERA_TURN_RATE;

    auto positionOffset = glm::vec3(0.0);

    if (w.getKeyHold(Window::Keys::UP))
        positionOffset.z -= speed;
    if (w.getKeyHold(Window::Keys::DOWN))
        positionOffset.z += speed;
    if (w.getKeyHold(Window::Keys::LEFT))
        positionOffset.x -= speed;
    if (w.getKeyHold(Window::Keys::RIGHT))
        positionOffset.x += speed;
    if (w.getKeyHold(Window::Keys::FORWARD))
        positionOffset.y += speed;
    if (w.getKeyHold(Window::Keys::BACKWARD))
        positionOffset.y -= speed;

    positionOffset = glm::rotate(glm::mat4(1.0f), this->m_orientation.y, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(positionOffset, 1);
    this->m_position += positionOffset;
}