//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "physicsState.h"
PhysicsState::PhysicsState(const float &delta_t,
                           const float &mass,
                           const float &momentOfInertia,
                           const std::vector<float>& position,
                           const std::vector<float>& orientation,
                           const std::vector<float>& velocity,
                           const std::vector<float>& angularVelocity) :
        delta_t(delta_t), mass(mass), momentOfInertia(momentOfInertia),
        maximumRadius(0.0f), position(position), orientation(orientation),
        velocity(velocity), angularVelocity(angularVelocity)
{
    this->acceleration = { ZERO_VECTOR };
    this->torque = { ZERO_VECTOR };
    this->angularAcceleration = { ZERO_VECTOR };
}


std::pair<std::vector<float>, std::vector<float>> PhysicsState::getDifferenceInStates(const PhysicsState &p1, const PhysicsState &p2) {
    std::vector<float> deltaPosition, deltaOrientation;
    deltaPosition.reserve(DIMENSIONS);
    deltaOrientation.reserve(DIMENSIONS);
    for (uint8_t i = 0; i < DIMENSIONS; i ++) {
        deltaPosition.push_back(p2.position[i] - p1.position[i]);
        deltaOrientation.push_back(p2.orientation[i] - p1.orientation[i]);
    }
    return { deltaPosition, deltaOrientation };

}

void PhysicsState::setNewPos(const std::vector<float> &newPos) {
    assert(newPos.size() == DIMENSIONS);
    for (uint8_t i = 0; i < DIMENSIONS; i ++) this->position[i] = newPos[i];
}

void PhysicsState::setNewOrientation(const std::vector<float> &newOrientation) {
    assert(newOrientation.size() == DIMENSIONS);
    for (uint8_t i = 0; i < DIMENSIONS; i ++) this->orientation[i] = newOrientation[i];
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

void PhysicsState::changeState(const std::vector<float>& _acceleration,
                               const std::vector<float>& _torque,
                               const std::vector<float>& _angularAcceleration) {
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
    std::vector<float> normalVec = {
            p->getPositionOfCM()[0] - this->getPositionOfCM()[0],
            p->getPositionOfCM()[1] - this->getPositionOfCM()[1],
            p->getPositionOfCM()[2] - this->getPositionOfCM()[2]
    };
    float length = sqrt((normalVec[0] * normalVec[0]) +
                        (normalVec[1] * normalVec[1]) +
                        (normalVec[2] * normalVec[2]));
    if (length == 0.0f) return; // Avoid division by zero
    for (float& i : normalVec) i /= length;

    // Compute relative velocity along the normal
    float relativeVelocity = 0.0f;
    for (int i = 0; i < DIMENSIONS; i++) {
        relativeVelocity += (p->velocity[i] - this->velocity[i]) * normalVec[i];
    }

    // Skip collision response if objects are moving apart
    if (relativeVelocity > 0) return;


    // Compute normal and tangential components of velocity for this object
    float v1_dot_n = 0.0f;
    for (int i = 0; i < DIMENSIONS; i++) v1_dot_n += this->velocity[i] * normalVec[i];

    std::vector<float> v1in, v1it;
    for (int i = 0; i < DIMENSIONS; i++) {
        v1in.push_back(v1_dot_n * normalVec[i]);
        v1it.push_back(this->velocity[i] - v1in[i]);
    }

    // Compute normal and tangential components of velocity for the other object
    float v2_dot_n = 0.0f;
    for (int i = 0; i < DIMENSIONS; i++) v2_dot_n += p->velocity[i] * normalVec[i];

    std::vector<float> v2in, v2it;
    for (int i = 0; i < DIMENSIONS; i++) {
        v2in.push_back(v2_dot_n * normalVec[i]);
        v2it.push_back(p->velocity[i] - v2in[i]);
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

    std::vector<float> v1f, v2f;
    for (int i = 0; i < DIMENSIONS; i++) {
        v1f.push_back((m1 - m2) * v1in[i] / totalMass + (2 * m2 * v2in[i]) / totalMass);
        v2f.push_back((m2 - m1) * v2in[i] / totalMass + (2 * m1 * v1in[i]) / totalMass);
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