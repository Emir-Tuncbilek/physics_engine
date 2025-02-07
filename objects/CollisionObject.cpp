//
// Created by Emir Tuncbilek on 12/30/24.
//

#include "CollisionObject.h"

CollisionObject::CollisionObject(const std::string &path) : RenderObject(path) {
    const std::vector<float> zeros = { ZERO_VECTOR };
    // this->physics = std::make_shared<PhysicsState>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, pos, orient, zeros, zeros);
    this->xScale = 1.0f;
    this->yScale = 1.0f;
    this->zScale = 1.0f;
}

std::vector<std::shared_ptr<RenderObject>> CollisionObject::getObjects()  {
    std::vector<std::shared_ptr<RenderObject>> objects = { shared_from_this() };
    return objects;
}

void CollisionObject::resize(const float &x, const float &y, const float &z) {
    this->xScale = x;
    this->yScale = y;
    this->zScale = z;
}

void CollisionObject::render(glm::mat4 &view, glm::mat4 &projPersp) {
    auto mvp = projPersp * view;
    glm::vec3 correctedPosition =
            glm::vec3(this->physics->getPositionOfCM()[0],
                      this->physics->getPositionOfCM()[2],
                      -this->physics->getPositionOfCM()[1]
            ); // Swap Y and Z, negate Z for OpenGL
    if (!this->renderOffset.empty())
        correctedPosition += glm::vec3(this->renderOffset[0], this->renderOffset[2], -this->renderOffset[1]);
    mvp = glm::translate(mvp, correctedPosition);
    mvp = glm::rotate(mvp, this->physics->getOrientation()[0], glm::vec3(1.0f, 0.0f, 0.0f));
    mvp = glm::rotate(mvp, this->physics->getOrientation()[1], glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = glm::rotate(mvp, this->physics->getOrientation()[2], glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = glm::scale(mvp, glm::vec3(this->xScale, this->zScale, this->yScale));

    this->modelShaderProgram->use();
    glUniformMatrix4fv(this->mvpModelLocation, 1, GL_FALSE, &mvp[0][0]);
    if (this->modelTexture)
        this->modelTexture->use();
    this->model3D->draw();
}

std::shared_ptr<RenderObject> CollisionObject::clone() const {
    return std::make_shared<CollisionObject>(*this);
}
