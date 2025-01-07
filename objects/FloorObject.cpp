//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "FloorObject.h"

void FloorObject::init(const std::shared_ptr<PhysicsState>& p) {
    RenderObject::init(p);
    // setup the bounding boxes for collision detection
    float width  = 2.0f;
    float height = width, depth = width; // Suppose that it's a cube
    this->addBoundingVolume(std::make_shared<BoundingBox>(width, depth, height));
}

std::vector<std::shared_ptr<RenderObject>> FloorObject::getObjects()  {
    std::vector<std::shared_ptr<RenderObject>> objects = { shared_from_this()};
    return objects;
}

void FloorObject::render(glm::mat4 &view, glm::mat4 &projPersp) {
    const PhysicsState oldState = *this->physics;
    this->physics->reset();
    auto mvp = projPersp * view;
    if (this->context->renderObjects) {
        this->physics->reset();
        glm::vec3 correctedPosition =
                glm::vec3(this->physics->o_position[0],
                          this->physics->o_position[2],
                          -this->physics->o_position[1]
                ); // Swap Y and Z, negate Z for OpenGL
        mvp = glm::translate(mvp, correctedPosition);

        this->modelShaderProgram->use();
        glUniformMatrix4fv(this->mvpModelLocation, 1, GL_FALSE, &mvp[0][0]);
        if (this->modelTexture)
            this->modelTexture->use();
        this->model3D->draw();
    }

    this->renderCollisionMesh(view, projPersp, oldState);
}

std::shared_ptr<RenderObject> FloorObject::clone() const {
    return std::make_shared<FloorObject>(*this);
}
