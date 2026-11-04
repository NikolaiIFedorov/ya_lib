
#pragma once

#include <cstdint>

// HEADER: Logging
// Add: device logging

// HEADER: Event
// Support: positional output

// HEADER: Dt
// Support: Odom

// HEADER: Auton
// Support: auton selection
// Add: auton compiler

class ya_lib {
  public:
    struct Config {
        uint8_t tickMs;
    };
    static inline Config config = {10};

    static void init();
    static void compInit();
    static void disabled();
    static void auton();
    static void drivercontrol();
};
