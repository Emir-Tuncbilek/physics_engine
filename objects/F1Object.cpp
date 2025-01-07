//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "F1Object.h"

void F1Object::init(const std::shared_ptr<PhysicsState>& p) {
    RenderObject::init(p);

    // setup the bounding boxes for collision detection
    float width  = 2.0f; // this->physics->maximumRadius;
    float height = width, depth = width;    // Suppose that it's in fact a cube
    this->addBoundingVolume(std::make_shared<BoundingBox>(width, depth, height));
}

std::vector<std::shared_ptr<RenderObject>> F1Object::getObjects() {
    std::vector<std::shared_ptr<RenderObject>> objects = { shared_from_this() };
    return objects;
}

void F1Object::render(glm::mat4 &view, glm::mat4 &projPersp) {
    const PhysicsState oldState = *this->physics;
    if (this->context->renderObjects) {
        auto mvp = projPersp * view;
        glm::vec3 correctedPosition =
                glm::vec3(this->physics->getPositionOfCM()[0],
                          this->physics->getPositionOfCM()[2],
                          -this->physics->getPositionOfCM()[1]
                ); // Swap Y and Z, negate Z for OpenGL
        mvp = glm::translate(mvp, correctedPosition);
        mvp = glm::rotate(mvp, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        mvp = glm::rotate(mvp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        mvp = glm::rotate(mvp, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        this->modelShaderProgram->use();
        glUniformMatrix4fv(this->mvpModelLocation, 1, GL_FALSE, &mvp[0][0]);
        if (this->modelTexture)
            this->modelTexture->use();
        this->model3D->draw();
    }
    this->renderCollisionMesh(view, projPersp, oldState);
}

std::shared_ptr<RenderObject> F1Object::clone() const {
    return std::make_shared<F1Object>(*this);
}
