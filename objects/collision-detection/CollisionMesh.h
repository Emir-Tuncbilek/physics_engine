//
// Created by Emir Tuncbilek on 12/26/24.
//

#ifndef PHYSICS_ENGINE_COLLISIONMESH_H
#define PHYSICS_ENGINE_COLLISIONMESH_H

#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "./BoundingVolume.h"
#include "../../physics/physicsState.h"

class CollisionMesh {
public:
    CollisionMesh() {}
    ~CollisionMesh() = default;
    /* Necessary functions */
    void addBoundingVolume(const std::shared_ptr<BoundingVolume>& volume);
    std::pair<bool, std::vector<float>> checkCollision(const CollisionMesh& other) const;
    std::unique_ptr<CollisionMesh> clone() const;
    void nudgeCollisionMesh(const std::vector<float>& offset);
    void resize(const float& x, const float& y, const float& z);
    /* cool functions */
    void render(glm::mat4 &view, glm::mat4 &projPersp);
    void selfFit(); // creates a "best fit" collision mesh
public:
    std::shared_ptr<PhysicsState> parentPhysics;
private:
    std::vector<std::shared_ptr<BoundingVolume>> volumes;
};


#endif //PHYSICS_ENGINE_COLLISIONMESH_H
