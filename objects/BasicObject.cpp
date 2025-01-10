//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "BasicObject.h"

void BasicObject::init(const std::shared_ptr<PhysicsState>& p) {
    RenderObject::init(p);
    // setup the bounding boxes for collision detection
    float width  = 2.0f; // this->physics->maximumRadius;
    float height = width, depth = width;    // Suppose that it's in fact a cube
    this->addBoundingVolume(std::make_shared<BoundingBox>(width, depth, height));
}

std::vector<std::shared_ptr<RenderObject>> BasicObject::getObjects() {
    std::vector<std::shared_ptr<RenderObject>> objects = { shared_from_this() };
    return objects;
}

void BasicObject::render(glm::mat4 &view, glm::mat4 &projPersp) {
    const PhysicsState oldState = *this->physics;
    if (this->context->renderObjects) {
        auto mvp = projPersp * view;
        glm::vec3 correctedPosition =
                glm::vec3(this->physics->getPositionOfCM()[0],
                          this->physics->getPositionOfCM()[2],
                          -this->physics->getPositionOfCM()[1]
                ); // Swap Y and Z, negate Z for OpenGL
        if (!this->renderOffset.empty()) {
            correctedPosition += glm::vec3(this->renderOffset[0], this->renderOffset[2], -this->renderOffset[1]);
        }
        mvp = glm::translate(mvp, correctedPosition);
        mvp = glm::rotate(mvp, this->physics->getOrientation()[0], glm::vec3(1.0f, 0.0f, 0.0f));
        mvp = glm::rotate(mvp, this->physics->getOrientation()[1], glm::vec3(0.0f, 1.0f, 0.0f));
        mvp = glm::rotate(mvp, this->physics->getOrientation()[2], glm::vec3(0.0f, 0.0f, 1.0f));
        mvp = glm::scale(mvp, 0.001f * glm::vec3(1.0f));
        this->modelShaderProgram->use();
        glUniformMatrix4fv(this->mvpModelLocation, 1, GL_FALSE, &mvp[0][0]);
        if (this->modelTexture)
            this->modelTexture->use();
        this->model3D->draw();
        if (this->modelTexture)
            this->modelTexture->unuse();
    }
    this->renderCollisionMesh(view, projPersp, oldState);
}

std::shared_ptr<RenderObject> BasicObject::clone() const {
    return std::make_shared<BasicObject>(*this);
}
