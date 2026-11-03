#pragma once

#include "ffpi.hpp"
#include "limiter.hpp"
#include "pid.hpp"
#include "rotational.hpp"
#include "ya_lib.hpp"
#include <optional>

class Compensation {
  public:
    struct Config {
        Pid::Config pidConfig;
        ffPi::Config ffPiConfig;
        Limiter::Config limiterConfig;
    };
    mutable Config config;

    Compensation(std::optional<Rotational> rotational, float gearRatio, Config config);
    std::optional<float> getVoltage(float motorPosition, float motorVelocity) const;

    void setTargetPosition(std::optional<float> targetPosition, float motorVelocity) const;
    void setTargetVelocity(std::optional<float> targetVelocity, float motorVelocity) const;

  private:
    const float dt = ya_lib::config.tickMs * 0.001;
    mutable std::optional<float> targetVelocity = std::nullopt;
    ffPi velocityLoop;
    Pid positionLoop;
    mutable float previousVelocity = 0;

    Limiter limiter;

    std::optional<Rotational> rotational = std::nullopt;
    float gearRatio = 1;
};