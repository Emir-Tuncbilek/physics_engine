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

class CollisionMesh {
public:
    CollisionMesh() {}
    ~CollisionMesh() = default;
    /* Necessary functions */
    void addBoundingVolume(const std::shared_ptr<BoundingVolume>& volume);
    bool checkCollision(const CollisionMesh& other) const;
    std::unique_ptr<CollisionMesh> clone() const;
    void translate(const std::vector<float>& offset);
    void rotate(const std::vector<std::vector<float>>& rotationMatrix);
    void rotate(const std::vector<float>& angleDiff);
    /* cool functions */
    void render(glm::mat4 &view, glm::mat4 &projPersp);
    void selfFit();
private:
    std::vector<std::shared_ptr<BoundingVolume>> volumes;
};


#endif //PHYSICS_ENGINE_COLLISIONMESH_H
