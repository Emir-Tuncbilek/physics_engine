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

BoundingVolume::BoundingVolume(std::vector<float> geometricCenter) : center(geometricCenter), renderObject(nullptr) {
    this->orientation = {0.0f, 0.0f, 0.0f};
}

BoundingVolume::BoundingVolume(const BoundingVolume &boundingVolume) {
    this->center = boundingVolume.center;
    this->orientation = boundingVolume.orientation;
    this->rotationMatrix = boundingVolume.rotationMatrix;
    this->renderObject = boundingVolume.renderObject != nullptr ? boundingVolume.renderObject->clone() : nullptr;
}

BoundingVolume::~BoundingVolume() = default;

void BoundingVolume::applyRotationMatrix(const std::vector<float>& vec, std::vector<float>& result) const {
    const std::vector<std::vector<float>> markedUpVec = {
            std::vector<float>({vec[0]}), std::vector<float>({vec[1]}), std::vector<float>({vec[2]}),
    };
    std::vector<std::vector<float>> res;
    for (int i = 0; i < DIMENSIONS; i ++) res.push_back(std::vector<float>({0.0f, 0.0f, 0.0f}));
    multiplyMatrices(this->rotationMatrix, markedUpVec, res);
    result = { res[0][0], res[1][0], res[2][0] };
}

void BoundingVolume::translate(const std::vector<float>& newPos) {
    assert(newPos.size() == DIMENSIONS);
    for (uint8_t i = 0; i < DIMENSIONS; i ++) this->center[i] = newPos[i];
    this->renderObject->physics->setNewPos(this->center);
}

void BoundingVolume::setRotationMatrix(const std::vector<std::vector<float>>& rotMatrix) {
    this->rotationMatrix = rotMatrix;
    std::vector<float> angleDelta = extractEulerAngles(this->rotationMatrix);
    for (uint8_t i = 0; i < DIMENSIONS; i ++) this->orientation[i] += angleDelta[i];
    this->renderObject->physics->setNewOrientation(this->orientation);
}

void BoundingVolume::resize(const float &x, const float &y, const float &z) {
    this->renderObject->resize(x, y, z);
}
