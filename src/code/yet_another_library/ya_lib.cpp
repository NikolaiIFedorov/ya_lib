#include "ya_lib.hpp"
#include "bot.hpp"
#include "components/controller.hpp"
#include "pros/rtos.hpp"

void ya_lib::init() {
    Bot::init(config);
};

void ya_lib::compInit() {
    Controller::executeEvents(Controller::Event::CompInit);
};
void ya_lib::disabled() {
    Controller::executeEvents(Controller::Event::Disabled);
};
void ya_lib::auton() {
    Controller::executeEvents(Controller::Event::Auton);
};
void ya_lib::drivercontrol() {
    Log::Trace([]() {
        Controller::executeEvents(Controller::Event::DriverControl);
        while (true) {
            Controller::executeEvents();
            pros::delay(config.tickMs);
        };
    });
};