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
    BoundingSphere(const float& radius);
    BoundingSphere(const BoundingSphere& boundingSphere) = default;
    ~BoundingSphere() = default;

    std::shared_ptr<BoundingVolume> clone() override;
    std::pair<bool, std::vector<float>> isCollidingWith(std::shared_ptr<BoundingVolume> boundingVolume) override;
    std::pair<bool, std::vector<float>> testNormals(std::shared_ptr<BoundingVolume> boundingVolume) override;
    float getMax(const std::vector<float>& axis) const override;
    float getMin(const std::vector<float>& axis) const override;
    void render(glm::mat4 &view, glm::mat4 &projPersp) const override;
};


#endif //PHYSICS_ENGINE_BOUNDINGSPHERE_H
