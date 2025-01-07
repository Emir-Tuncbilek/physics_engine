//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "ParticuleObject.h"


void ParticuleObject::init(const std::shared_ptr<PhysicsState>& p) {
    RenderObject::init(p);
    // setup the bounding boxes for collision detection
    float width  = 0.02f;
    float height = width, depth = width;    // Suppose that it's in fact a cube
    auto boundingBox = std::make_shared<BoundingBox>(width, depth, height);
    boundingBox->resize(0.01f, 0.01f, 0.01f);
    this->addBoundingVolume(boundingBox);

}

std::vector<std::shared_ptr<RenderObject>> ParticuleObject::getObjects() {
    std::vector<std::shared_ptr<RenderObject>> objects = { this->shared_from_this() };
    return objects;
}

void ParticuleObject::render(glm::mat4 &view, glm::mat4 &projPersp) {
    const PhysicsState oldState = *this->physics;
    if (this->context->renderObjects) {
        auto model = glm::mat4(1.0f);
        auto mvp = projPersp * view * model;
        glm::vec3 correctedPosition =
                glm::vec3(this->physics->getPositionOfCM()[0],
                          this->physics->getPositionOfCM()[2],
                          -this->physics->getPositionOfCM()[1]
                ); // Swap Y and Z, negate Z for OpenGL
        mvp = glm::translate(mvp, correctedPosition);
        mvp = glm::scale(mvp, 0.01f * glm::vec3(1));

        this->modelShaderProgram->use();
        glUniformMatrix4fv(this->mvpModelLocation, 1, GL_FALSE, &mvp[0][0]);
        if (this->modelTexture)
            this->modelTexture->use();
        this->model3D->draw();
    }
    this->renderCollisionMesh(view, projPersp, oldState);
}

std::shared_ptr<RenderObject> ParticuleObject::clone() const {
    return std::make_shared<ParticuleObject>(*this);
}
