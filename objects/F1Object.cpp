//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "F1Object.h"

void F1Object::init() {
    RenderObject::init();
    float zeros[3] = { ZERO_VECTOR };
    float orientation[3] = { M_PI/2, M_PI, M_PI/2 };    // 90˚, 180˚, 90˚
    this->physics = std::make_shared<PhysicsState>(0.0f, 10.0f, 0.0f, zeros, orientation, zeros, zeros);
    this->physics->computeMaximumDistance(this->model3D->vertexData, 3);
    // this->physics->maximumRadius *= 0.01f;   // because the model is scaled to 0.01
}

void F1Object::render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) {
    this->physics->updateTimeDelta(delta_t);
    this->physics->nextFrame();
    auto mvp = projPersp * view;
    mvp = glm::rotate(mvp, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mvp = glm::rotate(mvp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = glm::rotate(mvp, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mvp = glm::translate(mvp, glm::make_vec3(this->physics->getPositionOfCM()));
    // mvp = glm::scale(mvp, 0.01f * glm::vec3(1));
    this->modelShaderProgram->use();
    glUniformMatrix4fv(this->mvpModelLocation, 1, GL_FALSE, &mvp[0][0]);
    if (this->modelTexture)
        this->modelTexture->use();
    this->model3D->draw();
    std::cout << "# F1 Object:\n" << *this->physics << std::endl;
}