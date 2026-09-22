#pragma once
#include "bot.hpp"
#include "components/controller.hpp"
#include "pros/rtos.hpp"

class _ya_lib {
  public:
    static void init() {
        Bot::init();
    };

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
        Controller::executeEvents(Controller::Event::DriverControl);
        while (true) {
            Controller::executeEvents();
            pros::delay(10);
        };
    };
};