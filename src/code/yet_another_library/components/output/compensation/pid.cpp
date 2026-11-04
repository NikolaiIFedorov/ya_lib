#include "pid.hpp"
#include <optional>

Pid::Pid(Config config) : config(config) {};

float Pid::getOffset(float motorPosition) const {
    const float proportion = targetPosition.value() - motorPosition;
    const float derivative = (proportion - prevProportion) / dt;

    prevProportion = proportion;
    return proportion * config.proportion + derivative * config.derivative;
};

void Pid::setTargetPosition(std::optional<float> targetPosition) const {
    this->targetPosition = targetPosition;
    if (this->targetPosition == std::nullopt)
        prevProportion = 0;
};

std::optional<float> Pid::getTargetPosition() const {
    return targetPosition;
}