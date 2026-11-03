#include "ffpi.hpp"
#include "utils/utils.hpp"

float ffPi::getOffset(float motorPosition, float targetVelocity, float targetAcceleration) const {

    const float proportion = targetVelocity - motorPosition;

    if (integralOverflow == 0 || getSign(proportion) != integralOverflow)
        integral += proportion * dt;

    float offset = getSign(targetVelocity) * config.constant + targetVelocity * config.dynamic +
                   targetAcceleration * config.inertia + proportion * config.proportion +
                   integral * config.integral;

    float clampedOffset = std::clamp(offset, -1.0f, 1.0f);
    integralOverflow = offset > 1.0f ? 1 : offset < -1.0f ? -1 : 0;

    return clampedOffset;
};