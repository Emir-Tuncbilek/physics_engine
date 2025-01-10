//
// Created by Emir Tuncbilek on 12/26/24.
//

#include "BoundingVolume.h"
#include "../RenderObject.h"

inline std::vector<float> extractEulerAngles(const std::vector<std::vector<float>>& R) {
    float pitch, yaw, roll;

    // Check for Gimbal Lock
    if (std::abs(R[2][0]) != 1) {
        // General case
        pitch = std::asin(-R[2][0]);
        yaw = std::atan2(R[1][0], R[0][0]);
        roll = std::atan2(R[2][1], R[2][2]);
    } else {
        // Gimbal lock case
        yaw = 0;
        if (R[2][0] == -1) {
            pitch = M_PI / 2;
            roll = std::atan2(-R[0][1], R[1][1]);
        } else {
            pitch = -M_PI / 2;
            roll = std::atan2(R[0][1], -R[1][1]);
        }
    }

    return {roll, pitch, yaw};
}

BoundingVolume::BoundingVolume() : renderObject(nullptr) {}

BoundingVolume::BoundingVolume(const BoundingVolume &boundingVolume) {
    this->rotationMatrix = boundingVolume.rotationMatrix;
    this->renderObject = boundingVolume.renderObject != nullptr ? boundingVolume.renderObject->clone() : nullptr;
    this->parentPhysics = boundingVolume.parentPhysics;
}

BoundingVolume::~BoundingVolume() = default;

void BoundingVolume::setPhysics(const std::shared_ptr<PhysicsState> &p) {
    this->parentPhysics = p;
    this->renderObject->physics = this->parentPhysics;
}

void BoundingVolume::nudge(const std::vector<float>& offset) {
    this->_offset = offset;
    this->renderObject->renderOffset = this->_offset;
}

void BoundingVolume::resize(const float &x, const float &y, const float &z) {
    this->renderObject->resize(x, y, z);
}
