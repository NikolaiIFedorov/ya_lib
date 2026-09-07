#include "output.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

void Output::operator()(float arg) const {
    _call(port, arg);
};

float Output::getState() {
    return _getState(port);
};

Motor::Motor(Brain::Port port, bool flipped) : Output(port, spin, getVoltage) {
    if (prosPortPortMap.contains(port))
        motors.insert({port, pros::Motor(prosPortPortMap.at(port))});
    else
        Output::~Output(); // Warn

    if (flipped)
        flippedMotors.insert(port);
}

void Motor::spin(Brain::Port port, float pct) {
    motors.at(port).move_voltage(12000 * (flippedMotors.contains(port) ? -pct : pct));
};

float Motor::getVoltage(Brain::Port port) {
    return motors.at(port).get_voltage();
};

Piston::Piston(Brain::Port port, bool defState) : Output(port, extend, getExtended) {
    if (prosPortPortMap.contains(port))
        pistons.insert({port, pros::ADIPneumatics(prosPortPortMap.at(port), defState)});
    else
        Output::~Output(); // Warn
}

void Piston::extend(Brain::Port port, float extended) {
    pistons.at(port).set_value(extended);
};

float Piston::getExtended(Brain::Port port) {
    return pistons.at(port).is_extended();
};