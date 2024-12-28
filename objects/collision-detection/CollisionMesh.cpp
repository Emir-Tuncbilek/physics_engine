//
// Created by Emir Tuncbilek on 12/26/24.
//

#include "CollisionMesh.h"

void CollisionMesh::addBoundingVolume(const std::shared_ptr<BoundingVolume>& volume) {
    this->volumes.push_back(volume);
}

bool CollisionMesh::checkCollision(const CollisionMesh &other) const {
    for (auto && volume : this->volumes) {
        for (auto && otherVolume : other.volumes) {
            if (volume->isCollidingWith(otherVolume)) return true;
        }
    }
    return false;
}

std::unique_ptr<CollisionMesh> CollisionMesh::clone() const {
    std::unique_ptr<CollisionMesh> cm = std::make_unique<CollisionMesh>();
    for (auto && volume : this->volumes) {
        cm->addBoundingVolume(volume->clone());
    }
    return std::move(cm);
}

void CollisionMesh::translate(const std::vector<std::vector<float>>& translationMatrix) {
    for (auto && volume : this->volumes) volume->translate(translationMatrix);
}

void CollisionMesh::rotate(const std::vector<std::vector<float>>& rotationMatrix) {
    for (auto && volume : this->volumes) volume->setRotationMatrix(rotationMatrix);
}

void CollisionMesh::render() {
    /* Todo: implement */
}

void CollisionMesh::selfFit() {
    /* Todo: implement */
}

