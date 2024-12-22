//
// Created by Emir Tuncbilek on 12/11/24.
//

#include "./forces.h"
#include "sstream"

Force::Force(const float force[], const float pointOfApplication[]) {
    if (force == nullptr) throw std::invalid_argument("\"force\" argument should not be null.");
    std::copy(force, force + DIMENSIONS, this->_force);
    if (pointOfApplication == nullptr) this->_pointOfApplication = nullptr;
    else std::copy(pointOfApplication, pointOfApplication + DIMENSIONS, this->_pointOfApplication);
}

Force::Force(const Force& force) {
    std::copy(force._force, force._force + DIMENSIONS, this->_force);
    std::copy(force._pointOfApplication, force._pointOfApplication + DIMENSIONS, this->_pointOfApplication);
}

void Force::apply(std::shared_ptr<PhysicsState>& physicsState) {
    // a force causes an acceleration of the CM, a change in torque and a change in acceleration
    // F = m . a -> a = F/m
    float newAcceleration[3];
    std::copy(this->_force, this->_force + DIMENSIONS, newAcceleration);
    for (float& a : newAcceleration) { a /= (float) physicsState->getMass(); }

    // M = r x F
    // |  i    j    k   |
    // | r(0) r(1) r(2) |
    // | F(0) F(1) F(2) | -> [r(1)F(2) - F(1)r(2)]i - [r(0)F(2) - F(0)r(2)]j + [r(0)F(1) - F(0)r(1)]k

    // r -> distance from CM, i.e, POA - positionCM
    float r[3] = {0.0f, 0.0f, 0.0f};    // if this->_pointOfApplication is nullptr, the force is applied at the CM
    if (this->_pointOfApplication != nullptr) {
        r[0] = this->_pointOfApplication[0] - physicsState->getPositionOfCM()[0];
        r[1] = this->_pointOfApplication[1] - physicsState->getPositionOfCM()[1];
        r[2] = this->_pointOfApplication[2] - physicsState->getPositionOfCM()[2];
    }
    const float torque[3] = {
            r[1] * this->_force[2] - this->_force[1] * r[2],
            -(r[0] * this->_force[2] - this->_force[0] * r[2]),
            r[0] * this->_force[1] - this->_force[0] * r[1]
    };

    // M = I . angular-a -> angular-a = M / I
    float newAngularAcceleration[3];
    std::copy(torque, torque + DIMENSIONS, newAngularAcceleration);
    for (float& a : newAngularAcceleration) { a /= (float) physicsState->getMomentOfInertia(); }

    physicsState->changeState(newAcceleration, torque, newAngularAcceleration);
}

void Force::apply(std::vector<std::shared_ptr<PhysicsState>>& physicsState) {
    for (std::shared_ptr<PhysicsState>& ps : physicsState) {
        this->apply(ps);
    }
}

ForceField::ForceField(const float *field) : Force(field, nullptr) {
    std::copy(field, field + DIMENSIONS, this->_forceField);
}

void ForceField::apply(std::shared_ptr<PhysicsState> &physicsState) {
    float zeros[3] = { ZERO_VECTOR };
    physicsState->changeState(this->_forceField, zeros, zeros);
}

void ForceField::apply(std::vector<std::shared_ptr<PhysicsState>> &physicsState) {
    for (std::shared_ptr<PhysicsState> state : physicsState) {
        this->apply(state);
    }
}