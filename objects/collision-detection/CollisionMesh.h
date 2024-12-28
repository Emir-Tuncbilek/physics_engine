//
// Created by Emir Tuncbilek on 12/26/24.
//

#ifndef PHYSICS_ENGINE_COLLISIONMESH_H
#define PHYSICS_ENGINE_COLLISIONMESH_H

#include <iostream>
#include <vector>
#include <memory>

#include "./BoundingVolume.h"

class CollisionMesh {
public:
    CollisionMesh() {}
    ~CollisionMesh() = default;
    /* Necessary functions */
    void addBoundingVolume(const std::shared_ptr<BoundingVolume>& volume);
    bool checkCollision(const CollisionMesh& other) const;
    std::unique_ptr<CollisionMesh> clone() const;
    void translate(const std::vector<std::vector<float>>&  translationMatrix);
    void rotate(const std::vector<std::vector<float>>& rotationMatrix);
    /* cool functions */
    void render();
    void selfFit();
private:
    std::vector<std::shared_ptr<BoundingVolume>> volumes;
};


#endif //PHYSICS_ENGINE_COLLISIONMESH_H
