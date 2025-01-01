//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "FloorObject.h"

void FloorObject::init() {
    RenderObject::init();
    std::vector<float> zeros = { ZERO_VECTOR };
    std::vector<float> position = { 0.0f, 0.0f, -3.0f };
    this->physics = std::make_shared<PhysicsState>(0.0f, std::numeric_limits<float>::infinity(), 0.0f, position, zeros, zeros, zeros);
    this->physics->computeMaximumDistance(this->model3D->vertexData, 3);

    // setup the bounding boxes for collision detection
    float width  = this->physics->maximumRadius * 1.0f;   // 0.707 is about sqrt(2)/2 which is sin(45˚)
    float height = width, depth = width;                    // Suppose that it's in fact a cube
    std::vector<float> pos;
    pos.reserve(DIMENSIONS);
    for (int i = 0; i < DIMENSIONS; i ++) pos.push_back(this->physics->getPositionOfCM()[i]);
    this->addBoundingVolume(std::make_shared<BoundingBox>(pos, width, depth, height));
    // this->physics->maximumRadius *= 0.01f;   // because the model is scaled to 0.01
}

std::vector<std::shared_ptr<RenderObject>> FloorObject::getObjects() const {
    std::vector<std::shared_ptr<RenderObject>> objects = { std::make_shared<FloorObject>(*this) };
    return objects;
}

void FloorObject::render(glm::mat4 &view, glm::mat4 &projPersp, const float &delta_t) {
    const PhysicsState oldState = *this->physics;
    this->physics->resetAccelerations();    // no acceleration
    auto mvp = projPersp * view;
    if (!this->context->renderCollisionMesh) {
        glm::vec3 correctedPosition =
                glm::vec3(this->physics->getPositionOfCM()[0],
                          this->physics->getPositionOfCM()[2],
                          -this->physics->getPositionOfCM()[1]
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
