#include "bot.hpp"
#include "bot/controller.hpp"

Bot::Equation::Equation(float val) : EquationFunction([val]() { return val; }), constant(true) {};

Bot::Outputs::Outputs(Output output) : OutputVector({output}) {};

void Bot::_compInit() {
    Controller::triggerCallback(Controller::Event::Comp_Init);
};

void Bot::_auton() {
    Controller::triggerCallback(Controller::Event::Auton);
};

void Bot::_drivercontrol() {
    Controller::pollInputs();
};
