#include "rotational.hpp"
#include <cmath>
#include <cstdlib>

Rotational::Rotational(Brain::Port port) : rotational(static_cast<uint8_t>(port)) {};

float Rotational::getAngle(bool continous) const {
    float rotations = rotational.get_position() / 36000.f;
    if (!continous) {
        rotations = std::fmod(rotations, 1.0f);
        if (rotations < 0.0f)
            rotations += 1.0f;
    }

    return rotations;
};

float Rotational::getVelocity() const {
    return rotational.get_velocity() / 3600.0f;
};

void Rotational::reset() {
    rotational.reset_position();
};
