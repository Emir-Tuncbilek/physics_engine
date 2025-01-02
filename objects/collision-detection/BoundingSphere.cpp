//
// Created by Emir Tuncbilek on 12/26/24.
//

#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(const std::vector<float>& center, float radius) : BoundingVolume(center), radius(radius) {}

std::shared_ptr<BoundingVolume> BoundingSphere::clone() {
    auto sphere = std::make_shared<BoundingSphere>(*this);
    return sphere;
}

std::pair<bool, std::vector<float>> BoundingSphere::isCollidingWith(std::shared_ptr<BoundingVolume> boundingVolume) {
    std::shared_ptr<BoundingSphere> object = std::make_shared<BoundingSphere>(*this);
    auto resultThis = this->testNormals(boundingVolume);
    if (resultThis.first) return resultThis;
    auto resultOther = boundingVolume->testNormals(object);
    if (resultOther.first) return resultOther;
    return { false, {} };
}

std::pair<bool, std::vector<float>> BoundingSphere::testNormals(std::shared_ptr<BoundingVolume> boundingVolume) {
    // Using SAT collision logic, a normal vector can be defined as any outgoing point from the geometric center.
    // The problem is clear : we have an infinite number of normals to test. We can try the following :
    // - define 1 normal vector from the center of the sphere to the center of the other bounding object.
    // - project both shapes onto the new-formed normal plane
    // - check if intersection.
    // This function will only check 1 axis of collision, and so if there is in fact a collision, but this method returns false,
    // we must hope that the second object returns true when checking its normals.

    std::vector<float> normalVec = {
            boundingVolume->getGeomCenter()[0] - this->center[0],
            boundingVolume->getGeomCenter()[1] - this->center[1],
            boundingVolume->getGeomCenter()[2] - this->center[2]
    };

    // normalize the normal vector
    const float normalLength = normalVec[0] * normalVec[0] + normalVec[1] * normalVec[1] + normalVec[2] * normalVec[2];
    const float inverseSqrtLength = quakeIIIFastInverseSqrt(normalLength);
    for (float & f : normalVec) f *= inverseSqrtLength;

    // detect collision
    float objectMin = this->getMin(normalVec),          objectMax = this->getMax(normalVec);
    float otherMin  = boundingVolume->getMin(normalVec), otherMax = boundingVolume->getMax(normalVec);
    bool result = SATIsCollision(objectMin, objectMax, otherMin, otherMax);
    if (result) return {result, normalVec};
    return {false, {}};
}

float BoundingSphere::getMin(const std::vector<float>& axis) const {
    // Project the center onto the axis and subtract the radius to get the minimum projection
    return dotProduct(this->center, axis, DIMENSIONS) - this->radius;
}

float BoundingSphere::getMax(const std::vector<float>& axis) const {
    // Project the center onto the axis and add the radius to get the maximum projection
    return dotProduct(this->center, axis, DIMENSIONS) + this->radius;
}

void BoundingSphere::render(glm::mat4 &view, glm::mat4 &projPersp) const {

}