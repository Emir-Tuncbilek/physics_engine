//
// Created by Emir Tuncbilek on 12/11/24.
//

#ifndef PHYSICS_ENGINE_FORCES_H
#define PHYSICS_ENGINE_FORCES_H

#include <iostream>
#include <memory>
#include <vector>

#include "./physicsState.h"

class Force {
protected:
    virtual void apply(std::shared_ptr<PhysicsState>& physicsState);

private:
    std::vector<float> _force;
    std::vector<float> _pointOfApplication;

public:
    Force(const std::vector<float>& force , const std::vector<float>& pointOfApplication);

    Force(const Force& f);

    ~Force() = default;

    virtual void apply(std::vector<std::shared_ptr<PhysicsState>>& physicsState);
};

class ForceField : public Force {
protected:
    void apply(std::shared_ptr<PhysicsState>& physicsState) override;

private:
    std::vector<float> _forceField;

public:
    ForceField(const std::vector<float>& field);

    void apply(std::vector<std::shared_ptr<PhysicsState>>& physicsState) override;
};

#endif //PHYSICS_ENGINE_FORCES_H
