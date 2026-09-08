#include "output.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

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
    if (prosPortFromPort.contains(port))
        motors.insert({port, pros::Motor(prosPortFromPort.at(port))});

    if (flipped)
        flippedOutputs.insert(port);
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
    if (prosPortFromPort.contains(port))
        pistons.insert({port, pros::ADIPneumatics(prosPortFromPort.at(port), defState)});

    if (flipped)
        flippedOutputs.insert(port);
}

void Piston::extend(Brain::Port port, float extended) {
    pistons.at(port).set_value(flippedOutputs.contains(port) ? !bool(extended) : extended);
};

float Piston::getExtended(Brain::Port port) {
    return pistons.at(port).is_extended();
};