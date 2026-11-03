#pragma once

#include "ya_lib.hpp"

class Limiter {
  public:
    struct Config {
        float tickRamp;
    };
    Config config;
    Limiter(Config config);

    float limit(float distance, float target, float present) const;
    float accelerate(float target, float present) const;

    void setRampTicks(float velocity) const;

  private:
    const float dt = ya_lib::config.tickMs * 0.001;
    mutable float rampTicks = 0;
};