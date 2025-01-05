//
// Created by Emir Tuncbilek on 12/11/24.
//

#ifndef PHYSICS_ENGINE_FORCES_H
#define PHYSICS_ENGINE_FORCES_H

#include <iostream>
#include <memory>
#include <vector>

#include "./physicsState.h"

#define GRAVITY_ACCELERATION_RATE 9.81f

class Force {

private:
    std::vector<float> _force;
    std::vector<float> _pointOfApplication;

public:
    Force(const std::vector<float>& force , const std::vector<float>& pointOfApplication);

    Force(const Force& f);

    ~Force() = default;

    virtual void apply(std::shared_ptr<PhysicsState>& physicsState);

    virtual void apply(std::vector<std::shared_ptr<PhysicsState>>& physicsState);
};

class ForceField : public Force {
private:
    std::vector<float> _forceField;

public:
    ForceField(const std::vector<float>& field);

    void apply(std::shared_ptr<PhysicsState>& physicsState) override;

    void apply(std::vector<std::shared_ptr<PhysicsState>>& physicsState) override;
};

#endif //PHYSICS_ENGINE_FORCES_H
