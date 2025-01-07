//
// Created by Emir Tuncbilek on 12/21/24.
//

#ifndef PHYSICS_ENGINE_CLOUDPARTICULEOBJECT_H
#define PHYSICS_ENGINE_CLOUDPARTICULEOBJECT_H

#include "./RenderObject.h"
#include "./ParticuleObject.h"

class CloudParticuleObject : public RenderObject {
public:
    explicit CloudParticuleObject(const std::string& path,
                                  size_t numParticules,
                                  std::pair<float, float> maxWidth, // x
                                  std::pair<float, float> maxDepth, // y
                                  std::pair<float, float> maxHeight // z
                                  ):
            RenderObject(),
            particuleObjectPath(path),
            _numParticules(numParticules),
            height(maxHeight), width(maxWidth), depth(maxDepth) {
        assert(this->width.first  <= this->width.second);
        assert(this->height.first <= this->height.second);
        assert(this->depth.first  <= this->depth.second);
    }

    CloudParticuleObject(const CloudParticuleObject& other);

    ~CloudParticuleObject() = default;

    void init(const std::shared_ptr<PhysicsState>& p) override;

    void reset() override;

    std::vector<std::shared_ptr<RenderObject>> getObjects() override;

    void setContextFromScene(const std::shared_ptr<SceneContext> &sceneContext) override;

    void render(glm::mat4 &view, glm::mat4 &projPersp) override;

    std::vector<std::shared_ptr<PhysicsState>> getPhysicState() override;

    std::shared_ptr<RenderObject> clone() const override;

    void updatePhysics(const float &delta_t) override;

private:
    std::string particuleObjectPath;
    size_t _numParticules;
    std::pair<float, float> height, width, depth;
    std::vector<std::shared_ptr<RenderObject>> particules;
};


#endif //PHYSICS_ENGINE_CLOUDPARTICULEOBJECT_H
