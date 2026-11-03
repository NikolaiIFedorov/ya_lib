#include "inertial.hpp"

Inertial::Inertial(Brain::Port port) : inertial(static_cast<uint8_t>(port)) {};

float Inertial::getRotation() {
    return inertial.get_rotation();
};