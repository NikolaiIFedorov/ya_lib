#include "limiter.hpp"
#include "math.hpp"
#include <algorithm>
#include <cmath>

Limiter::Limiter(Config config) : config(config) {};

float Limiter::accelerate(float targetVelocity, float currentVelocity) const {
    rampTicks += Math::getSign(targetVelocity - currentVelocity);
    const float center = currentVelocity / config.tickRamp;
    rampTicks = std::clamp(rampTicks, center - 2, center + 2);

    return std::clamp(
        rampTicks * config.tickRamp,
        -std::abs(targetVelocity),
        std::abs(targetVelocity));
};

float Limiter::limit(float distance, float targetVelocity, float currentVelocity) const {
    const float acceleration = config.tickRamp / dt;
    const float macDeaccelSpeed = std::sqrt(2 * acceleration * std::abs(distance));
    return accelerate(
        std::copysign(std::min(std::abs(targetVelocity), macDeaccelSpeed), targetVelocity),
        currentVelocity);
};

void Limiter::setRampTicks(float velocity) const {
    rampTicks = velocity / config.tickRamp;
}
