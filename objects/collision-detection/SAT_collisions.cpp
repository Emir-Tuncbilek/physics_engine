//
// Created by Emir Tuncbilek on 12/27/24.
//

#include <iostream>

#include "SAT_collisions.h"

bool SATIsCollision(const float & min1,
                      const float & max1,
                      const float & min2,
                      const float & max2) {
    // Let A be index 1 and B, index 2. We must check if any portion of b is in A:
    // (i.e.) A-B-B-A, B-A-B-A, A-B-A-B  or B-A-A-B
    // A non-collision would be either A-A-B-B or B-B-A-A
    return !((max1 < min2) || (max2 < min1));   // determines if there is a
}

float dotProduct(const std::vector<float>& vec1, const std::vector<float>& vec2, const uint8_t& dimensions) {
    float product = 0.0f;
    for (u_int8_t i = 0; i < dimensions; i++) { product += vec1[i] * vec2[i]; }
    return product;
}

float quakeIIIFastInverseSqrt(const float& x) {
    // this is an algorithm, that is not mine, that was developed for efficiently computing the inverse sqrt
    long i;
    float x2, y;
    const float threeHalves = 1.5f;
    x2 = x * 0.5f;
    y = x;
    i = * ( long *) &y;
    i = EVIL_CONSTANT - (i >> 1);
    y = * ( float *) &i;
    y = y * (threeHalves - x2 * y * y);
    return y;
}

void normalize(std::vector<float>& vec) {
    assert(!vec.empty());
    float magnitudeSquared = 0.0f;
    for (auto && param : vec) magnitudeSquared += param * param;
    const float invSqrt = quakeIIIFastInverseSqrt(magnitudeSquared);
    for (auto && param : vec) param *= invSqrt;
}

void crossProduct3(const std::vector<float>& v1, const std::vector<float>& v2, std::vector<float>& result) {
    assert(v1.size() == 3 && v2.size() == 3);
    result.clear();
    result.reserve(v1.size());
    const float i = v1[1] * v2[2] - v2[1] * v1[2];
    const float j = v1[0] * v2[2] - v2[0] * v1[2];
    const float k = v1[0] * v2[1] - v2[0] * v1[1];
    result.push_back(i); result.push_back(-j); result.push_back(k);

}

void multiplyMatrices(const std::vector<std::vector<float>>& m1, const std::vector<std::vector<float>>& m2, std::vector<std::vector<float>>& result) {
    for (size_t i = 0; i < m1.size(); i ++) {
        for (size_t j = 0; j < m2[0].size(); j ++) {
            result[i][j] = m1[i][0] * m2[0][j] + m1[i][1] * m2[1][j] + m1[i][2] * m2[2][j];
        }
    }
}