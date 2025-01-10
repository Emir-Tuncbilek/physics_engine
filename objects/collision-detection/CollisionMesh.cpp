//
// Created by Emir Tuncbilek on 12/26/24.
//

#include "CollisionMesh.h"

void CollisionMesh::addBoundingVolume(const std::shared_ptr<BoundingVolume>& volume) {
    volume->parentPhysics = this->parentPhysics;
    volume->setPhysics(this->parentPhysics);
    this->volumes.push_back(volume);
}

std::pair<bool, std::vector<float>> CollisionMesh::checkCollision(const CollisionMesh &other) const {
    for (auto && volume : this->volumes) {
        for (auto && otherVolume : other.volumes) {
            auto collisionVerdict = volume->isCollidingWith(otherVolume);
            if (collisionVerdict.first) {
                return collisionVerdict;
            }
        }
    }
    return { false, { 0.0f }};
}

std::unique_ptr<CollisionMesh> CollisionMesh::clone() const {
    std::unique_ptr<CollisionMesh> cm = std::make_unique<CollisionMesh>();
    for (auto && volume : this->volumes) {
        cm->addBoundingVolume(volume->clone());
    }
    return std::move(cm);
}

void CollisionMesh::nudgeCollisionMesh(const std::vector<float> &offset) {
    for (auto&& volume : this->volumes) volume->nudge(offset);
}

void CollisionMesh::resize(const float &x, const float &y, const float &z) {
    for (auto&& volume : this->volumes) volume->resize(x, y, z);
}

void CollisionMesh::render(glm::mat4 &view, glm::mat4 &projPersp) {
    for (auto && volume : this->volumes) volume->render(view, projPersp);
}

void CollisionMesh::selfFit() {
    /* Todo: implement */
}

