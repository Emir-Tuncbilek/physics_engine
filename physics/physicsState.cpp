//
// Created by Emir Tuncbilek on 12/21/24.
//

#include "physicsState.h"
#include "./forces.h"

PhysicsState::PhysicsState(const float &delta_t,
                           const float &mass,
                           const float &momentOfInertia,
                           const float &elasticity,
                           const float &friction,
                           const std::vector<float>& position,
                           const std::vector<float>& orientation,
                           const std::vector<float>& velocity,
                           const std::vector<float>& angularVelocity) :
        delta_t(delta_t), mass(mass), momentOfInertia(momentOfInertia),
        maximumRadius(0.0f), position(position), orientation(orientation),
        velocity(velocity), angularVelocity(angularVelocity)
{
    assert (elasticity <= 1.0f && elasticity >= 0.0f);
    assert (friction <= 1.0f && friction >= 0.0f);
    this->elasticity = elasticity;
    this->friction = friction;
    this->acceleration = { ZERO_VECTOR };
    this->torque = { ZERO_VECTOR };
    this->angularAcceleration = { ZERO_VECTOR };
    this->o_position = this->position;
    this->o_velocity = this->velocity;
    this->o_orientation = this->orientation;
    this->o_angularVelocity = this->angularVelocity;
    this->immobile = false;
}

void PhysicsState::reset() {
    this->velocity = this->o_velocity;
    this->position = this->o_position;
    this->orientation = this->o_orientation;
    this->angularVelocity = this->o_angularVelocity;
    this->immobile = false;
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
    for (uint8_t i = 0; i < DIMENSIONS; i ++) {
        this->o_position[i] = newPos[i];
        this->position[i] = newPos[i];
    }
}

void PhysicsState::incrementPositionBy(const std::vector<float> &positionChange) {
    assert(positionChange.size() == DIMENSIONS);
    for (uint8_t i = 0; i < DIMENSIONS; i ++) this->position[i] += positionChange[i];
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

void PhysicsState::applyCollision(std::shared_ptr<PhysicsState> &p, const std::vector<float> &normal) {
    std::vector<float> relVelocity;
    float relSpeed = 0.0f;

    // Calculate the relative velocity along the normal
    for (uint8_t i = 0; i < DIMENSIONS; i++) {
        relVelocity.push_back(this->velocity[i] - p->velocity[i]);
        relSpeed+= relVelocity[i] * normal[i];
    }

    const float bounciness = std::min(this->elasticity, p->elasticity);

    // If either object has infinite mass, it is immovable, so no impulse is applied to that object

    if (this->mass == std::numeric_limits<float>::infinity()) {
        for (uint8_t i = 0; i < DIMENSIONS; i ++) {
            float sign = normal[i] > 0.0f ? -1.0f : 1.0f;
            p->velocity[i] = normal[i] != 0.0f ? sign * bounciness * normal[i] * p->velocity[i] : p->velocity[i];
        }
        return;
    }
    if (p->mass == std::numeric_limits<float>::infinity()) {
        for (uint8_t i = 0; i < DIMENSIONS; i ++) {
            float sign = normal[i] > 0.0f ? -1.0f : 1.0f;
            this->velocity[i] = normal[i] != 0.0f ? sign * bounciness * normal[i] * this->velocity[i] : this->velocity[i];
        }
        return;
    }

    // compute the reflection vector for both objects:
    // R = D - 2(D . N)N, where D is the direction (normalized velocity), and N is the normal
    std::vector<float> dirThis  = this->velocity; normalize(dirThis);
    std::vector<float> dirOther = this->velocity; normalize(dirOther);
    const float reflectionFactorThis  = 2.0f * dotProduct(dirThis, normal, DIMENSIONS);
    const float reflectionFactorOther = 2.0f * dotProduct(dirOther, normal, DIMENSIONS);

    std::vector<float> reflectionThis = {
            dirThis[0] - reflectionFactorThis * normal[0],
            dirThis[1] - reflectionFactorThis * normal[1],
            dirThis[2] - reflectionFactorThis * normal[2]
    };

    std::vector<float> reflectionOther = {
            dirOther[0] - reflectionFactorOther * normal[0],
            dirOther[1] - reflectionFactorOther * normal[1],
            dirOther[2] - reflectionFactorOther * normal[2]
    };

    normalize(reflectionThis); normalize(reflectionOther);

    // compute the change in velocities, along the collision axis
    const float v1i = getModule({
        this->velocity[0] * normal[0],
        this->velocity[1] * normal[1],
        this->velocity[2] * normal[2],
    });
    const float v2i = getModule({
        p->velocity[0] * normal[0],
        p->velocity[1] * normal[1],
        p->velocity[2] * normal[2],
    });

    auto [ v1f, v2f ] = solveLinearFinalVelocitiesAfterCollision(v1i, v2i, this->mass, p->mass);

    const float deltaV1 = v1f - v1i;
    const float deltaV2 = v2f - v2i;

    for (uint8_t i = 0; i < DIMENSIONS; i ++) {
        this->velocity[i] += bounciness * deltaV1 * reflectionThis[i];
        p->velocity[i] -= bounciness * deltaV2 * reflectionOther[i];
    }
}

void PhysicsState::applyFriction(std::shared_ptr<PhysicsState> &p) {

}

bool PhysicsState::isImmobile(PhysicsState &p) {
    this->immobile =  std::abs(this->velocity[0] - p.velocity[0]) <= MIN_VELOCITY_THRESHOLD &&
            std::abs(this->velocity[1] - p.velocity[1]) <= MIN_VELOCITY_THRESHOLD &&
            std::abs(this->velocity[2] - p.velocity[2]) <= MIN_VELOCITY_THRESHOLD;
    p.immobile = this->immobile;
    return this->immobile;
}


std::ostream& operator << (std::ostream& o, const PhysicsState& physicsState) {
    o << "Position: (" << physicsState.position[0] << ", " << physicsState.position[1] << ", " << physicsState.position[2] << ") m\n";
    o << "Velocity: (" << physicsState.velocity[0] << ", " << physicsState.velocity[1] << ", " << physicsState.velocity[2] << ") m/s\n";
    o << "Acceleration: (" << physicsState.acceleration[0] << ", " << physicsState.acceleration[1] << ", " << physicsState.acceleration[2] << ") m/s^2\n";

    return o;
}