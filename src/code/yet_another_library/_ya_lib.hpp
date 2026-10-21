#pragma once

#include "components/controller.hpp"
#include "pros/rtos.hpp"
#include "utils/utils.hpp"

// -TODO: Add pid
// -TODO: Add BaseOp util
// -TODO: Add PortInstance groups
// ?TODO: Add port pattern
// -TODO: Add pid
// -TODO: Add logging
// TODO: Add device logging
// TODO: Support Auton selection
// TODO: Add background stuff
// TODO: Improve code quality
// TODO: Improve syntax
// -TODO: Force System const
// TODO: Support Device
// TODO: Add auton compiler

class _ya_lib {
  public:
    static void init();

    static void compInit() {
        Controller::executeEvents(Controller::Event::CompInit);
    };
    static void disabled() {
        Controller::executeEvents(Controller::Event::Disabled);
    };
    static void auton() {
        Controller::executeEvents(Controller::Event::Auton);
    };
    static void drivercontrol() {
        Log::Trace([]() {
            Controller::executeEvents(Controller::Event::DriverControl);
            while (true) {
                Controller::executeEvents();
                pros::delay(tick);
            };
        });
    };
};
