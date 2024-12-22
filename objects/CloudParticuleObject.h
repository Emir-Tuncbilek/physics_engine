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
                                  float maxXDistance,
                                  float maxYDistance,
                                  float airSpeed) :
            RenderObject(),
            particuleObjectPath(path),
            _numParticules(numParticules),
            _maxXDistance(maxXDistance),
            _maxYDistance(maxYDistance),
            _airSpeed(airSpeed) {}

    ~CloudParticuleObject() = default;

    void init() override;

    void render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) override;

    std::vector<std::shared_ptr<PhysicsState>> getPhysicState() override;

private:
    std::string particuleObjectPath;
    size_t _numParticules;
    std::vector<std::unique_ptr<ParticuleObject>> particules;
    float _maxXDistance;
    float _maxYDistance;
    float _airSpeed;
};


#endif //PHYSICS_ENGINE_CLOUDPARTICULEOBJECT_H
