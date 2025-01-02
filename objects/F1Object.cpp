//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "F1Object.h"

void F1Object::init() {
    RenderObject::init();
    const std::vector<float> pos = { 0.0, 0.0f, 1.0f };
    const std::vector<float> zeros = { ZERO_VECTOR };
    const std::vector<float> orientation = { M_PI/2, M_PI, M_PI/2 };    // 90˚, 180˚, 90˚
    this->physics = std::make_shared<PhysicsState>(0.0f, 10.0f, 0.0f, pos, orientation, zeros, zeros);
    this->physics->computeMaximumDistance(this->model3D->vertexData, 3);

    // setup the bounding boxes for collision detection
    float width  = this->physics->maximumRadius * 1.0f;
    float height = width, depth = width;    // Suppose that it's in fact a cube
    std::vector<float> position;
    position.reserve(DIMENSIONS);
    for (int i = 0; i < DIMENSIONS; i ++) position.push_back(this->physics->getPositionOfCM()[i]);
    this->addBoundingVolume(std::make_shared<BoundingBox>(position, width, depth, height));
    // this->physics->maximumRadius *= 0.01f;   // because the model is scaled to 0.01
}

std::vector<std::shared_ptr<RenderObject>> F1Object::getObjects() const {
    std::vector<std::shared_ptr<RenderObject>> objects = { std::make_shared<F1Object>(*this) };
    return objects;
}

void F1Object::render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) {
    const PhysicsState oldState = *this->physics;
    this->physics->updateTimeDelta(delta_t);
    this->translateCollisionMeshToState();
    this->rotateCollisionMeshToState();
    if (!this->context->renderCollisionMesh) {
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
        // mvp = glm::scale(mvp, 0.01f * glm::vec3(1));
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
