#include "dt.hpp"

Dt::Dt(Motor left, Motor right, Config config)
    : SubSystem<float, float>([left, right](auto init) {
          init.addSubMacro(left, [](float left, float right) { return left; });
          init.addSubMacro(right, [](float left, float right) { return right; });
      }) {
    left.setConfig(config.compensationConfig);
    right.setConfig(config.compensationConfig);
};

void Dt::setOdoms(Odom x, Odom y) {
    xOdom.emplace(x);
    yOdom.emplace(y);
};

void Dt::setInertial(Inertial inertial) {
    this->inertial.emplace(inertial);
}

void Dt::operator()(float l, float r) {
    const float left = config.controlKind == ControlKind::Arcade ? l : l - r;
    const float right = config.controlKind == ControlKind::Arcade ? r : l + r;
    (*this)(left, right);
};

void Dt::operator()(float x, float y, float a) {

};
