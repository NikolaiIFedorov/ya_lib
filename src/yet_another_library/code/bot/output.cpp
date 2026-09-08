#include "output.hpp"
#include "../utils.hpp"
#include "pros/adi.hpp"
#include <unordered_set>

std::unordered_set<Brain::Port> Output::flippedOutputs = {};
std::map<Brain::Port, pros::Motor> Motor::motors = {};
std::map<Brain::Port, pros::adi::Pneumatics> Piston::pistons = {};

Output::Output(Brain::Port port, Call call, GetState getState)
    : port(port), _call(call), _getState(getState) {};

Output::Output(Brain::Port port, bool flipped) {
    *this = Motor(port, flipped);
};
Output::Output(bool defExtended, Brain::Port port, bool flipped) {
    *this = Piston(port, defExtended, flipped);
};

void Output::operator()(float arg) const {
    _call(port, arg);
};

float Output::getState() const {
    return _getState(port);
};

Motor::Motor(Brain::Port port, bool flipped) : Output(port, Call(spin), GetState(getVoltage)) {
    TRACE([port, flipped]() {
        if (prosPortFromPort.contains(port))
            motors.insert({port, pros::Motor(prosPortFromPort.at(port))});

        if (flipped)
            flippedOutputs.insert(port);
    });
}

// Add pid
void Motor::spin(Brain::Port port, float pct) {
    motors.at(port).move_voltage(12000 * (flippedOutputs.contains(port) ? -pct : pct));
};

float Motor::getVoltage(Brain::Port port) {
    return motors.at(port).get_voltage();
};

Piston::Piston(Brain::Port port, bool defState, bool flipped)
    : Output(port, Call(extend), GetState(getExtended)) {
    TRACE([port, flipped, defState]() {
        if (prosPortFromPort.contains(port))
            pistons.insert({port, pros::adi::Pneumatics(prosPortFromPort.at(port), defState)});

        if (flipped)
            flippedOutputs.insert(port);
    });
}

void Piston::extend(Brain::Port port, float extended) {
    pistons.at(port).set_value(flippedOutputs.contains(port) ? !bool(extended) : extended);
};

float Piston::getExtended(Brain::Port port) {
    return pistons.at(port).is_extended();
};