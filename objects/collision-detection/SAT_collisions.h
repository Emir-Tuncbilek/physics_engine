//
// Created by Emir Tuncbilek on 12/27/24.
//

#ifndef PHYSICS_ENGINE_SAT_COLLISIONS_H
#define PHYSICS_ENGINE_SAT_COLLISIONS_H

#include <iostream>
#include <array>
#include <vector>
#include <assert.h>
#include <cmath>
#include <cstdint>

#define EVIL_CONSTANT 0x5f3759df

bool SATIsCollision(const float & min1,
                         const float & max1,
                         const float & min2,
                         const float & max2);

float dotProduct(const std::vector<float>& vec1, const std::vector<float>& vec2, const uint8_t& dimensions);

float quakeIIIFastInverseSqrt(const float& x);

void normalize(std::vector<float>& vec);

float getModule(const std::vector<float>& vec);

void crossProduct3(const std::vector<float>& v1, const std::vector<float>& v2, std::vector<float>& result);

void multiplyMatrices(const std::vector<std::vector<float>>& m1, const std::vector<std::vector<float>>& m2, std::vector<std::vector<float>>& result);

/* returns v1f, v2f, assuming constant mass */
std::pair<float, float> solveLinearFinalVelocitiesAfterCollision(const float& v1i, const float& v2i, const float& m1, const float& m2);

#endif //PHYSICS_ENGINE_SAT_COLLISIONS_H
