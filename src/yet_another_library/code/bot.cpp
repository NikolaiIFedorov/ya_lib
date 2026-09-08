#include "bot.hpp"
#include "bot/controller.hpp"
#include "pros/rtos.hpp"
#include "utils.hpp"

Bot::Equation::Equation(float val) : EquationFunction([val]() { return val; }), constant(true) {};

Bot::Outputs::Outputs(Output output) : OutputVector({std::move(output)}) {};

void Bot::Outputs::operator()(float arg) const {
    TRACE([arg, this]() {
        const auto &thisOutputs = *this;
        for (const auto &output : thisOutputs)
            output(arg);
    });
}

float Bot::Outputs::getState() const {
    return TRACE([this]() {
        float totalState;
        const auto &thisOutputs = *this;
        for (const auto &output : thisOutputs)
            totalState += output.getState();

        return totalState / thisOutputs.size();
    });
};

void Bot::Outputs::addEvent(Controller::Event event, Equation equation) const {
    TRACE([this, event, equation]() {
        const auto &thisOutputs = *this;
        for (const auto &output : thisOutputs)
            Controller::addCallback(event, [output, equation, event]() {
                output(equation.constant ? equation() * Controller::getInput(event) : equation());
            });
    });
};

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
