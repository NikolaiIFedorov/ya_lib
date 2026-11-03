
#pragma once

#include <cstdint>

// TODO: Add Odom
// TODO: Add device logging
// TODO: Support Auton selection
// TODO: Add auton compiler

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
