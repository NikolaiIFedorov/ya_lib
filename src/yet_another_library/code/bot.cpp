#include "bot.hpp"
#include "bot/controller.hpp"
#include "pros/rtos.hpp"

Bot::Equation::Equation(float val) : EquationFunction([val]() { return val; }), constant(true) {};

Bot::Outputs::Outputs(Output output) : OutputVector({output}) {};

void Bot::Outputs::operator()(float arg) const {
    const auto &thisOutputs = *this;
    for (const auto &output : thisOutputs)
        output(arg);
}

float Bot::Outputs::getState() const {
    float totalState;
    const auto &thisOutputs = *this;
    for (const auto &output : thisOutputs)
        totalState += thisOutputs.getState();

    return totalState / thisOutputs.size();
};

void Bot::Outputs::addEvent(Controller::Event event, Equation equation) const {
    const auto &thisOutputs = *this;
    for (const auto &output : thisOutputs)
        Controller::addCallback(event, [output, equation]() { output(equation()); });
};

void Bot::_compInit() {
    Controller::triggerCallback(Controller::Event::Comp_Init);
};

void Bot::_auton() {
    Controller::triggerCallback(Controller::Event::Auton);
};

void Bot::_drivercontrol() {
    while (true) {
        Controller::pollInputs();
        pros::delay(10);
    }
};
