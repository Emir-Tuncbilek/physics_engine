//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "physicsState.h"

PhysicsState::PhysicsState(const float delta_t,
                           const float mass,
                           const float momentOfInertia,
                           const float *position,
                           const float *orientation,
                           const float *velocity,
                           const float *angularVelocity) :
    delta_t(delta_t), mass(mass), momentOfInertia(momentOfInertia), maximumRadius(0.0f)
{
    for (int i = 0; i < DIMENSIONS; i ++) {
        if (position + i != nullptr)        this->position[i]        = *(position + i);
        if (orientation + i != nullptr)     this->orientation[i]     = *(orientation + i);
        if (velocity + i != nullptr)        this->velocity[i]        = *(velocity + i);
        if (angularVelocity + i != nullptr) this->angularVelocity[i] = *(position + i);
    }
    // system is initially at rest
    const float zeros[3] = { ZERO_VECTOR };
    std::copy(zeros, zeros + DIMENSIONS, this->acceleration);
    std::copy(zeros, zeros + DIMENSIONS, this->angularAcceleration);
    std::copy(zeros, zeros + DIMENSIONS, this->torque);
}

void PhysicsState::computeMaximumDistance(std::vector<float> &vertices, int offset) {
    float furthestDistanceSquared = 0.0f;
    float geometricCenter[3] = {0.0f, 0.0f, 0.0f};
    if (vertices.size() % offset != 0) throw std::invalid_argument("Offset is invalid!");
    for (int i = 0; i < vertices.size(); i += offset) {
        geometricCenter[0] += vertices[i];
        geometricCenter[1] += vertices[i + 1];
        geometricCenter[2] += vertices[i + 2];
    }
    geometricCenter[0] /= (float)vertices.size() / (float)offset;
    geometricCenter[1] /= (float)vertices.size() / (float)offset;
    geometricCenter[2] /= (float)vertices.size() / (float)offset;


    for (int i = 0; i < vertices.size(); i += offset) {
        float dSquared = (vertices[i] - geometricCenter[0]) * (vertices[i] - geometricCenter[0]) +
                         (vertices[i + 1] - geometricCenter[1]) * (vertices[i + 1] - geometricCenter[1]) +
                         (vertices[i + 2] - geometricCenter[2]) * (vertices[i + 2] - geometricCenter[2]);
        if (dSquared > furthestDistanceSquared) furthestDistanceSquared = dSquared;
    }
    this->maximumRadius = sqrt(furthestDistanceSquared);
}

void PhysicsState::changeState(const float *_acceleration, const float *_torque, const float *_angularAcceleration) {
    for (int i = 0; i < DIMENSIONS; i ++ ) {
        this->acceleration[i]        += _acceleration[i];
        this->angularAcceleration[i] += _angularAcceleration[i];
        this->torque[i]              += _torque[i];
    }
    this->nextFrame();
}

void PhysicsState::resetAccelerations() {
    for (int i = 0; i < DIMENSIONS; i++) {
        this->acceleration[i]        = 0.0f;
        this->angularAcceleration[i] = 0.0f;
        this->torque[i]              = 0.0f;
    }
}

void PhysicsState::nextFrame() {
    // 1st compute position and orientation
    // position    = initial + (velocity * ∆t) + (1/2 * a * ∆t^2)
    // orientation = initial + (angular-v * ∆t) + (1/2 * angular-a * ∆t^2)
    for (int i = 0; i < DIMENSIONS; i ++) {
        this->position[i] += (this->velocity[i] * this->delta_t) + (0.5f * this->acceleration[i] * this->delta_t *
                                                                    this->delta_t);
        this->orientation[i] += (this->angularVelocity[i] * this->delta_t) + ( 0.5f * this->angularAcceleration[i] *
                                                                               this->delta_t * this->delta_t);
    }

    // 2nd compute new velocity and angular velocity
    // velocity  = initial + a * ∆t
    // angular-v = initial + angular-a * ∆t
    for (int i = 0; i < DIMENSIONS; i ++) {
        this->velocity[i]        += this->acceleration[i] * this->delta_t;
        this->angularVelocity[i] += this->angularAcceleration[i] * this->delta_t;
    }
}

void PhysicsState::applyCollision(std::shared_ptr<PhysicsState> &p)  {
    // Calculate the normal vector between the two centers of mass
    float normalVec[DIMENSIONS] = {
            p->getPositionOfCM()[0] - this->getPositionOfCM()[0],
            p->getPositionOfCM()[1] - this->getPositionOfCM()[1],
            p->getPositionOfCM()[2] - this->getPositionOfCM()[2]
    };
    float length = sqrt((normalVec[0] * normalVec[0]) +
                        (normalVec[1] * normalVec[1]) +
                        (normalVec[2] * normalVec[2]));
    if (length == 0.0f) return; // Avoid division by zero
    for (float& i : normalVec) i /= length;

    // Compute normal and tangential components of velocity for this object
    float v1_dot_n = 0.0f;
    for (int i = 0; i < DIMENSIONS; i++) v1_dot_n += this->velocity[i] * normalVec[i];

    float v1in[DIMENSIONS], v1it[DIMENSIONS];
    for (int i = 0; i < DIMENSIONS; i++) {
        v1in[i] = v1_dot_n * normalVec[i];
        v1it[i] = this->velocity[i] - v1in[i];
    }

    // Compute normal and tangential components of velocity for the other object
    float v2_dot_n = 0.0f;
    for (int i = 0; i < DIMENSIONS; i++) v2_dot_n += p->velocity[i] * normalVec[i];

    float v2in[DIMENSIONS], v2it[DIMENSIONS];
    for (int i = 0; i < DIMENSIONS; i++) {
        v2in[i] = v2_dot_n * normalVec[i];
        v2it[i] = p->velocity[i] - v2in[i];
    }

    // check if "p" is an immovable object
    if (p->getMass() == std::numeric_limits<float>::infinity()) {
        float v1f[3];
        for (int i = 0; i < DIMENSIONS; i++) {
            v1f[i] = -v1in[i];
            this->velocity[i] = v1f[i] + v1it[i];
        }
        return;
    }

    // check if "this" is an immovable object
    if (this->getMass() == std::numeric_limits<float>::infinity()) {
        float v2f[3];
        for (int i = 0; i < DIMENSIONS; i++) {
            v2f[i] = -v2in[i];
            p->velocity[i] = v2f[i] + v2it[i];
        }
        return;
    }

    // Compute final normal velocities using conservation of momentum
    const float m1 = this->getMass();
    const float m2 = p->getMass();
    float totalMass = m1 + m2;

    float v1f[DIMENSIONS], v2f[DIMENSIONS];
    for (int i = 0; i < DIMENSIONS; i++) {
        v1f[i] = (m1 - m2) * v1in[i] / totalMass + (2 * m2 * v2in[i]) / totalMass;
        v2f[i] = (m2 - m1) * v2in[i] / totalMass + (2 * m1 * v1in[i]) / totalMass;
    }

    // Combine final velocities (normal + tangential components)
    for (int i = 0; i < DIMENSIONS; i++) {
        this->velocity[i] = v1f[i] + v1it[i];
        p->velocity[i] = v2f[i] + v2it[i];
    }
}

std::ostream& operator << (std::ostream& o, const PhysicsState& physicsState) {
    o << "Position: (" << physicsState.position[0] << ", " << physicsState.position[1] << ", " << physicsState.position[2] << ") m\n";
    o << "Velocity: (" << physicsState.velocity[0] << ", " << physicsState.velocity[1] << ", " << physicsState.velocity[2] << ") m/s\n";
    o << "Acceleration: (" << physicsState.acceleration[0] << ", " << physicsState.acceleration[1] << ", " << physicsState.acceleration[2] << ") m/s^2\n";

    return o;
}