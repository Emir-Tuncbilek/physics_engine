//
// Created by Emir Tuncbilek on 12/21/24.
//

#ifndef PHYSICS_ENGINE_F1OBJECT_H
#define PHYSICS_ENGINE_F1OBJECT_H

#include "RenderObject.h"
#include "./collision-detection/BoundingBox.h"

class F1Object : public RenderObject {
public:
    explicit F1Object(const std::string& path) : RenderObject(path) {}

    F1Object(const F1Object& other) = default;

    ~F1Object() = default;

    void init() override;
    std::vector<std::shared_ptr<RenderObject>> getObjects() override;
    void render(glm::mat4 &view, glm::mat4 &projPersp) override;
    std::shared_ptr<RenderObject> clone() const override;
};


#endif //PHYSICS_ENGINE_F1OBJECT_H
