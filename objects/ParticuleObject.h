//
// Created by Emir Tuncbilek on 12/21/24.
//

#ifndef PHYSICS_ENGINE_PARTICULEOBJECT_H
#define PHYSICS_ENGINE_PARTICULEOBJECT_H

#include "./RenderObject.h"
#include "./collision-detection/BoundingBox.h"

class ParticuleObject : public RenderObject {
public:
    explicit ParticuleObject(const std::string& path) :
            RenderObject(path) {}

    ParticuleObject(const ParticuleObject& other) = default;
    ~ParticuleObject() = default;

    void init(const std::shared_ptr<PhysicsState>& p) override;
    std::vector<std::shared_ptr<RenderObject>> getObjects() override;
    void render(glm::mat4 &view, glm::mat4 &projPersp) override;
    std::shared_ptr<RenderObject> clone() const override;

};

#endif //PHYSICS_ENGINE_PARTICULEOBJECT_H
