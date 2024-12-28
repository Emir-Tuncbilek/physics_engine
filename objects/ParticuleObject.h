//
// Created by Emir Tuncbilek on 12/21/24.
//

#ifndef PHYSICS_ENGINE_PARTICULEOBJECT_H
#define PHYSICS_ENGINE_PARTICULEOBJECT_H

#include "./RenderObject.h"
#include "./collision-detection/BoundingBox.h"

class ParticuleObject : public RenderObject {
public:
    explicit ParticuleObject(const std::string& path, const float& xOffset, const float& yOffset) :
            RenderObject(path), XOffset(xOffset), YOffset(yOffset), speed(glm::vec3(0.0f)) {}

    ~ParticuleObject() = default;

    void init() override;
    std::vector<std::shared_ptr<RenderObject>> getObjects() const override;
    void render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) override;

public:
    glm::vec3 speed;
private:
    float XOffset;
    float YOffset;
};

#endif //PHYSICS_ENGINE_PARTICULEOBJECT_H
