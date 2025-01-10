//
// Created by Emir Tuncbilek on 12/11/24.
//

#ifndef PHYSICS_ENGINE_PHYSICSSTATE_H
#define PHYSICS_ENGINE_PHYSICSSTATE_H

#define DIMENSIONS 3
#define ZERO_VECTOR 0.0f, 0.0f, 0.0f
#define MIN_VELOCITY_THRESHOLD 0.1f

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ostream>
#include <cmath>
#include <memory>
#include <assert.h>

#include "../objects/collision-detection/SAT_collisions.h"

class PhysicsState : public std::enable_shared_from_this<PhysicsState> {
private:
    float delta_t;
    float mass;
    float momentOfInertia;
    float elasticity;                         // between 0.0f and 1.0f
    float friction;                           // between 0.0f and 1.0f
    std::vector<float> position;              // position of the CM
    std::vector<float> velocity;              // velocity of the CM
    std::vector<float> acceleration;          // acceleration of the CM
    std::vector<float> torque;
    std::vector<float> orientation;
    std::vector<float> angularVelocity;
    std::vector<float> angularAcceleration;

public:

    float maximumRadius;

    PhysicsState(const float &delta_t,
                 const float &mass,
                 const float &momentOfInertia,
                 const float &elasticity,
                 const float &friction,
                 const std::vector<float>& position,
                 const std::vector<float>& orientation,
                 const std::vector<float>& velocity,
                 const std::vector<float>& angularVelocity);

    ~PhysicsState() = default;

    PhysicsState(const PhysicsState& other) = default;

    void reset();

    /*
     * returns the position (first) and orientation (second)
     */
    static std::pair<std::vector<float>, std::vector<float>> getDifferenceInStates(const PhysicsState& p1, const PhysicsState& p2);

    void incrementPositionBy(const std::vector<float>& positionChange);

    void computeMaximumDistance(std::vector<float>& vertices, int offset);

    void changeState(const std::vector<float>& _acceleration,
                     const std::vector<float>& _torque,
                     const std::vector<float>& _angularAcceleration);

    void setNewPos(const std::vector<float>& newCenterPos);

    void setNewOrientation(const std::vector<float>& newCenterPos);

    void resetAccelerations();

    void nextFrame();

    void applyCollision(std::shared_ptr<PhysicsState>& p, const std::vector<float>& normal);

    void updateTimeDelta(const float time) { this->delta_t = time; }
    float getMass() const { return this->mass; }
    float getMomentOfInertia() const { return this->momentOfInertia; }
    std::vector<float> getPositionOfCM() const { return this->position; }
    std::vector<float> getOrientation() const { return this->orientation; }
    std::vector<float> getVelocityOfCM() const { return this->velocity; }

    bool isImmobile(PhysicsState& p);

    friend std::ostream& operator << (std::ostream& o, const PhysicsState& physicsState);

public:
    // original settings
    bool immobile;
    std::vector<float> o_position;
    std::vector<float> o_velocity;
    std::vector<float> o_orientation;
    std::vector<float> o_angularVelocity;

private:
    void applyFriction(std::shared_ptr<PhysicsState>& p);
};

#endif //PHYSICS_ENGINE_PHYSICSSTATE_H
