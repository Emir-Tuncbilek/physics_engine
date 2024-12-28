//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "ParticuleObject.h"

void ParticuleObject::init() {
    RenderObject::init();
    float zeros[3] = { ZERO_VECTOR };
    float pos[3] = { -5.0f, 1.0f + XOffset, 0.0f + YOffset };
    float velocity[3] = { this->speed.x, this->speed.y, this->speed.z };
    this->physics = std::make_shared<PhysicsState>(0.0f, 1.0f, 1.0f, pos, zeros, velocity, zeros);
    this->physics->computeMaximumDistance(this->model3D->vertexData, 3);
    this->physics->maximumRadius *= 0.01f;   // because the model is scaled to 0.01

    // setup the bounding boxes for collision detection
    float width  = this->physics->maximumRadius * 0.707f;   // 0.707 is about sqrt(2)/2 which is sin(45˚)
    float height = width, depth = width;                    // Suppose that it's in fact a cube
    std::vector<float> position;
    position.reserve(DIMENSIONS);
    for (int i = 0; i < DIMENSIONS; i ++) position.push_back(this->physics->getPositionOfCM()[i]);
    this->addBoundingVolume(std::make_shared<BoundingBox>(position, width, depth, height));
    // this->physics->maximumRadius *= 0.01f;   // because the model is scaled to 0.01

}

std::vector<std::shared_ptr<RenderObject>> ParticuleObject::getObjects() const {
    std::vector<std::shared_ptr<RenderObject>> objects = { std::make_shared<ParticuleObject>(*this) };
    return objects;
}

void ParticuleObject::render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) {
    this->physics->updateTimeDelta(delta_t);
    this->physics->nextFrame();
    this->rotateCollisionMeshToState();
    this->translateCollisionMeshToState();

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