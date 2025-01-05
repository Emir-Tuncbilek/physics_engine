//
// Created by Emir Tuncbilek on 12/11/24.
//

#include "./forces.h"
#include "sstream"

Force::Force(const std::vector<float>& force, const std::vector<float>& pointOfApplication) {
    assert(force.size() == DIMENSIONS);
    this->_force = force;
    if (pointOfApplication.empty()) return;
    this->_pointOfApplication = pointOfApplication;
}

Force::Force(const Force& force) {
    this->_force = force._force;
    this->_pointOfApplication = force._pointOfApplication;
}

void Force::apply(std::shared_ptr<PhysicsState>& physicsState) {
    // a force causes an acceleration of the CM, a change in torque and a change in acceleration
    // F = m . a -> a = F/m
    std::vector<float> newAcceleration = this->_force;
    // apply force in the normal direction if immobile
    // Todo: change this for force to be applied along a normal, not just in the opposite direction
    if (physicsState->immobile) for (auto && param : newAcceleration) param *= -1.0f;
    for (float& a : newAcceleration) { a /= (float) physicsState->getMass(); }

    // M = r x F
    // |  i    j    k   |
    // | r(0) r(1) r(2) |
    // | F(0) F(1) F(2) | -> [r(1)F(2) - F(1)r(2)]i - [r(0)F(2) - F(0)r(2)]j + [r(0)F(1) - F(0)r(1)]k

    // r -> distance from CM, i.e, POA - positionCM
    std::vector<float> r = {0.0f, 0.0f, 0.0f};    // if this->_pointOfApplication is empty, the force is applied at the CM
    if (!this->_pointOfApplication.empty()) {
        r[0] = this->_pointOfApplication[0] - physicsState->getPositionOfCM()[0];
        r[1] = this->_pointOfApplication[1] - physicsState->getPositionOfCM()[1];
        r[2] = this->_pointOfApplication[2] - physicsState->getPositionOfCM()[2];
    }
    const std::vector<float> torque = {
            r[1] * this->_force[2] - this->_force[1] * r[2],
            -(r[0] * this->_force[2] - this->_force[0] * r[2]),
            r[0] * this->_force[1] - this->_force[0] * r[1]
    };

    // M = I . angular-a -> angular-a = M / I
    std::vector<float> newAngularAcceleration = torque;
    for (float& a : newAngularAcceleration) { a /= (float) physicsState->getMomentOfInertia(); }

    physicsState->changeState(newAcceleration, torque, newAngularAcceleration);
    physicsState->immobile = false;
}

void Force::apply(std::vector<std::shared_ptr<PhysicsState>>& physicsState) {
    for (std::shared_ptr<PhysicsState>& ps : physicsState) {
        this->apply(ps);
    }
}

ForceField::ForceField(const std::vector<float>& field) : Force(field, {}) {
    this->_forceField = field;
}

void ForceField::apply(std::shared_ptr<PhysicsState> &physicsState) {
    if (physicsState->immobile) return;
    std::vector<float> zeros = { ZERO_VECTOR };
    physicsState->changeState(this->_forceField, zeros, zeros);
}

void ForceField::apply(std::vector<std::shared_ptr<PhysicsState>> &physicsState) {
    for (std::shared_ptr<PhysicsState> state : physicsState) {
        this->apply(state);
    }
}