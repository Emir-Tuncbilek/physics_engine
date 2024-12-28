//
// Created by Emir Tuncbilek on 12/26/24.
//

#include "BoundingVolume.h"

BoundingVolume::BoundingVolume(std::vector<float> geometricCenter) : center(geometricCenter) {}

BoundingVolume::BoundingVolume(const BoundingVolume &boundingVolume) {
    this->center = boundingVolume.center;
    this->rotationMatrix = boundingVolume.rotationMatrix;
}

void BoundingVolume::applyRotationMatrix(const std::vector<float>& vec, std::vector<float>& result) const {
    const std::vector<std::vector<float>> markedUpVec = {
            std::vector<float>({vec[0]}), std::vector<float>({vec[1]}), std::vector<float>({vec[2]}),
    };
    std::vector<std::vector<float>> res;
    for (int i = 0; i < DIMENSIONS; i ++) res.push_back(std::vector<float>({0.0f, 0.0f, 0.0f}));
    multiplyMatrices(this->rotationMatrix, markedUpVec, res);
    result = { res[0][0], res[1][0], res[2][0] };
}

void BoundingVolume::translate(const std::vector<std::vector<float>>& translationMatrix) {
    const std::vector<std::vector<float>> markedUpCenter = {
            { this->center[0] },
            { this->center[1] },
            { this->center[2] },
            { 0.0f }
    };
    std::vector<std::vector<float>> result = {
            {0.0f},
            {0.0f},
            {0.0f},
            {0.0f}
    };
    multiplyMatrices(translationMatrix, markedUpCenter, result);
    for (u_int8_t i = 0; i < DIMENSIONS; i ++) this->center[i] = result[i][0];
}

void BoundingVolume::setRotationMatrix(const std::vector<std::vector<float>>& rotMatrix) {
    this->rotationMatrix = rotMatrix;
}