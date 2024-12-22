//
// Created by Emir Tuncbilek on 12/21/24.
//

#ifndef PHYSICS_ENGINE_FLOOROBJECT_H
#define PHYSICS_ENGINE_FLOOROBJECT_H

#include "./RenderObject.h"

class FloorObject : public RenderObject {
public:
    explicit FloorObject(const std::string& path) : RenderObject(path) {}

    ~FloorObject() = default;

    void init() override;
    void render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) override;
};


#endif //PHYSICS_ENGINE_FLOOROBJECT_H
