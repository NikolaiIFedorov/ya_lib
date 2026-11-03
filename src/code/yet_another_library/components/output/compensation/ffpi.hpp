#pragma once

#include "ya_lib.hpp"
#include <cstdint>

class ffPi {
  public:
    struct Config {
        float proportion;
        float integral;
        float inertia;

        float constant;
        float dynamic;
    };
    mutable Config config;

    ffPi(Config config);

    float getOffset(float motorPosition, float targetVelocity, float targetAcceleration) const;

  private:
    const float dt = ya_lib::config.tickMs * 0.001;
    mutable float integral = 0;
    mutable int8_t integralOverflow = 0;
};
