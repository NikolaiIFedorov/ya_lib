#pragma once

#include "pros/colors.hpp"
#include <cstdint>
#include <string>

class Brain {
  public:
    enum class Port {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        _10,
        _11,
        _12,
        _13,
        _14,
        _15,
        _16,
        _17,
        _18,
        _19,
        _20
    };

    static void log(size_t line, std::string spacing, std::string msg, pros::Color color);
    static inline const uint8_t screenCapacity = 12;
};