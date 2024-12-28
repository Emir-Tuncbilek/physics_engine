//
// Created by Emir Tuncbilek on 12/26/24.
//

#ifndef PHYSICS_ENGINE_BOUNDINGBOX_H
#define PHYSICS_ENGINE_BOUNDINGBOX_H

#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>

#include "./BoundingVolume.h"

#define NUM_VERTICES 8
#define NUM_NORMALS 6

class BoundingBox : public BoundingVolume {
private:
    float width;    // x
    float depth;    // y
    float height;   // z

public:
    BoundingBox(const std::vector<float>& center, const float& width, const float& depth, const float& height);
    BoundingBox(const BoundingBox& boundingBox) = default;
    ~BoundingBox() = default;

    std::shared_ptr<BoundingVolume> clone() override;
    bool isCollidingWith(std::shared_ptr<BoundingVolume> boundingVolume) override;
    bool testNormals(std::shared_ptr<BoundingVolume> boundingVolume) override;
    float getMax(const std::vector<float>& axis) const override;
    float getMin(const std::vector<float>& axis) const override;

private:
    float getPositionOf(const std::vector<float>& axis, const std::function<bool(float, float)>& comparator) const;
};


#endif //PHYSICS_ENGINE_BOUNDINGBOX_H
