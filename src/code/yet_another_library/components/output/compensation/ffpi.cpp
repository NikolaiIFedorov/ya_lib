#include "ffpi.hpp"
#include "math.hpp"
#include <algorithm>

ffPi::ffPi(Config config) : config(config) {};

float ffPi::getOffset(float motorPosition, float targetVelocity, float targetAcceleration) const {
    const float proportion = targetVelocity - motorPosition;

    if (integralOverflow == 0 || Math::getSign(proportion) != integralOverflow)
        integral += proportion * dt;

    float offset = Math::getSign(targetVelocity) * config.constant +
                   targetVelocity * config.dynamic + targetAcceleration * config.inertia +
                   proportion * config.proportion + integral * config.integral;

    float clampedOffset = std::clamp(offset, -1.0f, 1.0f);
    integralOverflow = offset > 1.0f ? 1 : offset < -1.0f ? -1 : 0;

    return clampedOffset;
};