#include "bot.hpp"
#include "bot/controller.hpp"
#include "pros/rtos.hpp"
#include "utils.hpp"

Bot::Equation::Equation(float val) : EquationFunction([val]() { return val; }), constant(true) {};

Controller::Callback Bot::getAuton() {
    return TRACE([]() {
        const auto &autons = Controller::callbackFromEvent[Controller::Event::Auton];
        if (autons.size() == 1)
            return autons[0];

        return Log::getAuton(autons);
    });
};

void Bot::_compInit() {
    TRACE([]() {
        if (Controller::callbackFromEvent.contains(Controller::Event::Auton)) {
            const auto &auton = getAuton();
            Controller::callbackFromEvent[Controller::Event::Auton] = {auton};
        };

        Controller::triggerCallback(Controller::Event::Comp_Init);
    });
}
void Bot::_auton() {
    Controller::triggerCallback(Controller::Event::Auton);
};

void Bot::_drivercontrol() {
    TRACE([]() {
        while (true) {
            Controller::pollInputs();
            pros::delay(10);
        }
    });
};
