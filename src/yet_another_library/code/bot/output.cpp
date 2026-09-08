#include "output.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

Output::Output(
    Brain::Port port,
    std::function<void(Brain::Port, float)> call,
    std::function<float(Brain::Port)> getState)
    : port(port), _call(call), _getState(getState) {};

Output::Output(Brain::Port port, bool flipped) {
    if (Motor::prosPortPortMap.contains(port))
        *this = Motor(port, flipped);
    else
        *this = Piston(port, flipped);
};
Output::Output(bool defExtended, Brain::Port port, bool flipped) {
    if (Piston::prosPortPortMap.contains(port))
        *this = Piston(port, defExtended, flipped);
    else
        *this = Motor(port, flipped);
};

void Output::operator()(float arg) const {
    _call(port, arg);
};

float Output::getState() const {
    return _getState(port);
};

Motor::Motor(Brain::Port port, bool flipped) : Output(port, Call(spin), GetState(getVoltage)) {
    if (prosPortPortMap.contains(port))
        motors.insert({port, pros::Motor(prosPortPortMap.at(port))});

    if (flipped)
        flippedMotors.insert(port);
}

void Motor::spin(Brain::Port port, float pct) {
    motors.at(port).move_voltage(12000 * (flippedMotors.contains(port) ? -pct : pct));
};

float Motor::getVoltage(Brain::Port port) {
    return motors.at(port).get_voltage();
};

Piston::Piston(Brain::Port port, bool defState, bool flipped)
    : Output(port, Call(extend), GetState(getExtended)) {
    if (prosPortPortMap.contains(port))
        pistons.insert({port, pros::ADIPneumatics(prosPortPortMap.at(port), defState)});

    if (flipped)
        flippedPistons.insert(port);
}

void Piston::extend(Brain::Port port, float extended) {
    pistons.at(port).set_value(flippedPistons.contains(port) ? !bool(extended) : extended);
};

float Piston::getExtended(Brain::Port port) {
    return pistons.at(port).is_extended();
};