//
// Created by Emir Tuncbilek on 12/26/24.
//

#ifndef PHYSICS_ENGINE_BOUNDINGSPHERE_H
#define PHYSICS_ENGINE_BOUNDINGSPHERE_H

#include <iostream>
#include <vector>

#include "./BoundingVolume.h"

class BoundingSphere : public BoundingVolume {
private:
    float radius;
public:
    BoundingSphere(const std::vector<float>& center, float radius);
    BoundingSphere(const BoundingSphere& boundingSphere) = default;
    ~BoundingSphere() = default;

    std::shared_ptr<BoundingVolume> clone() override;
    bool isCollidingWith(std::shared_ptr<BoundingVolume> boundingVolume) override;
    bool testNormals(std::shared_ptr<BoundingVolume> boundingVolume) override;
    float getMax(const std::vector<float>& axis) const override;
    float getMin(const std::vector<float>& axis) const override;
};


#endif //PHYSICS_ENGINE_BOUNDINGSPHERE_H
