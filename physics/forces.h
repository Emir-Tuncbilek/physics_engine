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
    float _force[3];
    float * _pointOfApplication;

public:
    Force(const float force[], const float pointOfApplication[]);

    Force(const Force& f);

    ~Force() = default;

    virtual void apply(std::vector<std::shared_ptr<PhysicsState>>& physicsState);
};

class ForceField : public Force {
protected:
    void apply(std::shared_ptr<PhysicsState>& physicsState) override;

private:
    float _forceField[3];

public:
    ForceField(const float field[]);

    void apply(std::vector<std::shared_ptr<PhysicsState>>& physicsState) override;
};

#endif //PHYSICS_ENGINE_FORCES_H
