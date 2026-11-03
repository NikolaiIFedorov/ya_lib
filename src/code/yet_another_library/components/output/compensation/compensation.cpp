#include "compensation.hpp"
#include "ffpi.hpp"
#include <optional>

Compensation::Compensation(std::optional<Rotational> rotational, float gearRatio, Config config)
    : rotational(rotational), gearRatio(gearRatio), positionLoop(config.pidConfig),
      velocityLoop(config.ffPiConfig), limiter(config.limiterConfig) {};

std::optional<float> Compensation::getVoltage(float motorPosition, float motorVelocity) const {
    const float position =
        rotational.has_value() ? rotational->getAngle() / gearRatio : motorPosition;
    const float velocity =
        rotational.has_value() ? rotational->getVelocity() / gearRatio : motorVelocity;

    float velocityTarget;
    float targetAcceleration;
    if (positionLoop.getTargetPosition().has_value()) {
        velocityTarget = limiter.limit(
            positionLoop.getTargetPosition().value() - position,
            positionLoop.getOffset(position),
            velocity);
        targetAcceleration = std::clamp(
            (velocityTarget - previousVelocity) / dt,
            -limiter.config.tickRamp / dt,
            limiter.config.tickRamp / dt);
    } else if (targetVelocity.has_value()) {
        velocityTarget = targetVelocity.value();
        targetAcceleration = 0;
    } else
        return std::nullopt;

    previousVelocity = velocityTarget;
    return velocityLoop.getOffset(velocity, velocityTarget, targetAcceleration);
}

void Compensation::setTargetPosition(std::optional<float> targetPosition, float motorVelocity)
    const {
    positionLoop.setTargetPosition(targetPosition);
    if (positionLoop.getTargetPosition().has_value())
        targetVelocity = std::nullopt;

    limiter.setRampTicks(motorVelocity);
};

void Compensation::setTargetVelocity(std::optional<float> targetVelocity, float motorVelocity)
    const {
    this->targetVelocity = targetVelocity;
    if (this->targetVelocity.has_value())
        positionLoop.setTargetPosition(std::nullopt);

    limiter.setRampTicks(motorVelocity);
};