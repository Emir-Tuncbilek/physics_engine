//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "FloorObject.h"

void FloorObject::init() {
    RenderObject::init();
    float zeros[3] = { ZERO_VECTOR };
    float position[3] = { 0.0f, 0.0f, -3.0f };
    this->physics = std::make_shared<PhysicsState>(0.0f, std::numeric_limits<float>::infinity(), 0.0f, position, zeros, zeros, zeros);
    this->physics->computeMaximumDistance(this->model3D->vertexData, 3);
}

void FloorObject::render(glm::mat4 &view, glm::mat4 &projPersp, const float &delta_t) {
    this->physics->resetAccelerations();    // no acceleration
    auto mvp = projPersp * view;
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