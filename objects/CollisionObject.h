//
// Created by Emir Tuncbilek on 12/30/24.
//

#ifndef PHYSICS_ENGINE_COLLISIONOBJECT_H
#define PHYSICS_ENGINE_COLLISIONOBJECT_H

#include <iostream>
#include "RenderObject.h"

class CollisionObject : public RenderObject {
public:
    explicit CollisionObject(const std::string& path, const std::vector<float>& position, const std::vector<float>& orientation);

    CollisionObject(const CollisionObject& other) = default;

    ~CollisionObject() = default;

    std::vector<std::shared_ptr<RenderObject>> getObjects() override;
    void resize(const float& x, const float& y, const float& z) override;
    void init() override;
    void render(glm::mat4 &view, glm::mat4 &projPersp) override;
    std::shared_ptr<RenderObject> clone() const override;
private:
    float xScale, yScale, zScale;
};


#endif //PHYSICS_ENGINE_COLLISIONOBJECT_H
