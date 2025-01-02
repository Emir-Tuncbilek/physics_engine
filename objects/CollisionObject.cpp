//
// Created by Emir Tuncbilek on 12/30/24.
//

#include "CollisionObject.h"

CollisionObject::CollisionObject(const std::string &path, const std::vector<float> &position,
                                 const std::vector<float> &orientation) : RenderObject(path) {
    const std::vector<float> zeros = { ZERO_VECTOR };
    const std::vector<float> pos = {
            position[0], position[1], position[2]
    };
    const std::vector<float> orient = {
            orientation[0], orientation[1], orientation[2]
    };
    this->physics = std::make_shared<PhysicsState>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, pos, orient, zeros, zeros);
    this->xScale = 1.0f;
    this->yScale = 1.0f;
    this->zScale = 1.0f;
}

void CollisionObject::init() {
    RenderObject::init();
}

std::vector<std::shared_ptr<RenderObject>> CollisionObject::getObjects() const {
    std::vector<std::shared_ptr<RenderObject>> objects = { std::make_shared<CollisionObject>(*this) };
    return objects;
}

void CollisionObject::resize(const float &x, const float &y, const float &z) {
    this->xScale = x;
    this->yScale = y;
    this->zScale = z;
}

void CollisionObject::render(glm::mat4 &view, glm::mat4 &projPersp, const float &delta_t) {
    auto mvp = projPersp * view;
    glm::vec3 correctedPosition =
            glm::vec3(this->physics->getPositionOfCM()[0],
                      this->physics->getPositionOfCM()[2],
                      -this->physics->getPositionOfCM()[1]
            ); // Swap Y and Z, negate Z for OpenGL
    mvp = glm::translate(mvp, correctedPosition);
    mvp = glm::rotate(mvp, this->physics->getOrientation()[0], glm::vec3(1.0f, 0.0f, 0.0f));
    mvp = glm::rotate(mvp, this->physics->getOrientation()[1], glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = glm::rotate(mvp, this->physics->getOrientation()[2], glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = glm::scale(mvp, glm::vec3(this->xScale, this->yScale, this->zScale));

    this->modelShaderProgram->use();
    glUniformMatrix4fv(this->mvpModelLocation, 1, GL_FALSE, &mvp[0][0]);
    if (this->modelTexture)
        this->modelTexture->use();
    this->model3D->draw();
}

std::shared_ptr<RenderObject> CollisionObject::clone() const {
    return std::make_shared<CollisionObject>(*this);
}
