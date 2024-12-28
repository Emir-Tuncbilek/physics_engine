//
// Created by Emir Tuncbilek on 12/11/24.
//

#ifndef PHYSICS_ENGINE_PHYSICSSTATE_H
#define PHYSICS_ENGINE_PHYSICSSTATE_H

#define DIMENSIONS 3
#define ZERO_VECTOR 0.0f, 0.0f, 0.0f

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ostream>
#include <cmath>
#include <memory>

class PhysicsState {
private:
    float delta_t;
    float mass;
    float momentOfInertia;
    float position[3];              // position of the CM
    float velocity[3];              // velocity of the CM
    float acceleration[3];          // acceleration of the CM
    float torque[3];
    float orientation[3];
    float angularVelocity[3];
    float angularAcceleration[3];

public:

    float maximumRadius;

    PhysicsState(const float& delta_t,
                 const float& mass,
                 const float& momentOfInertia,
                 const float position[3],
                 const float orientation[3],
                 const float velocity[3],
                 const float angularVelocity[3]);

    ~PhysicsState() = default;

    void computeMaximumDistance(std::vector<float>& vertices, int offset);

    void changeState(const float _acceleration[3],
                     const float _torque[3],
                     const float _angularAcceleration[3]);

    void resetAccelerations();

    void nextFrame();

    void applyCollision(std::shared_ptr<PhysicsState>& p);

    void updateTimeDelta(const float time) { this->delta_t = time; }
    float getMass() const { return this->mass; }
    float getMomentOfInertia() const { return this->momentOfInertia; }
    float * getPositionOfCM() { return this->position; }
    float * getOrientation() { return this->orientation; }

    friend std::ostream& operator << (std::ostream& o, const PhysicsState& physicsState);
};

#endif //PHYSICS_ENGINE_PHYSICSSTATE_H
