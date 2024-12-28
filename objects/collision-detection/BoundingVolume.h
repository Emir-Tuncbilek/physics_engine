//
// Created by Emir Tuncbilek on 12/26/24.
//

#ifndef PHYSICS_ENGINE_BOUNDINGVOLUME_H
#define PHYSICS_ENGINE_BOUNDINGVOLUME_H

#include <iostream>
#include <memory>
#include <algorithm>
#include <cassert>
#include <array>

#include "./SAT_collisions.h"

#define DIMENSIONS 3

class BoundingVolume {
protected:
    std::vector<float> center;  // Replaces float* center
    std::vector<std::vector<float>> rotationMatrix;
public:
    BoundingVolume(std::vector<float> geometricCenter);
    BoundingVolume(const BoundingVolume& boundingVolume);
    ~BoundingVolume() = default;

    const std::vector<float>& getGeomCenter() { return this->center; }
    void translate (const std::vector<std::vector<float>>& translationMatrix);
    void applyRotationMatrix(const std::vector<float>& vec, std::vector<float>& result) const;

    void setRotationMatrix(const std::vector<std::vector<float>>& rotMatrix);

    virtual std::shared_ptr<BoundingVolume> clone() = 0;
    virtual bool isCollidingWith(std::shared_ptr<BoundingVolume> boundingVolume) = 0;
    virtual bool testNormals(std::shared_ptr<BoundingVolume> boundingVolume) = 0;
    virtual float getMin(const std::vector<float>& axis) const = 0;
    virtual float getMax(const std::vector<float>& axis) const = 0;
};


#endif //PHYSICS_ENGINE_BOUNDINGVOLUME_H
