
#pragma once

#include <cstdint>

// TODO: Add Odom
// TODO: Make Compensation adaptive
// TODO: Add device logging
// TODO: Support Auton selection
// TODO: Add auton compiler
// TODO: Expand macros

class ya_lib {
  public:
    struct Config {
        // How long one tick lasts.
        uint8_t tickMs;
    };
    static inline Config config = {10};

    static void init();
    static void compInit();
    static void disabled();
    static void auton();
    static void drivercontrol();
};
