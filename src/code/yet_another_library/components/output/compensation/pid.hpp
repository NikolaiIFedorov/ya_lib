#pragma once

#include "ya_lib.hpp"
#include <optional>

class Pid {
  public:
    struct Config {
        float proportion;
        float integral;
        float derivative;
    };
    mutable Config config;

    Pid(Config config);

    void setTargetPosition(std::optional<float> targetPosition) const;
    std::optional<float> getTargetPosition() const;

    float getOffset(float motorPosition) const;

  private:
    mutable std::optional<float> targetPosition = std::nullopt;

    mutable float prevProportion = 0;
    const float dt = ya_lib::config.tickMs * 0.001;
};