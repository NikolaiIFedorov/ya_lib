#include "output.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

Motor::Motor(Brain::Port port, bool flipped) : Output(port, spin) {
    if (prosPortPortMap.contains(port))
        motors.insert({port, pros::Motor(prosPortPortMap.at(port))});
    else
        Output::~Output(); // Warn

    if (flipped)
        flippedMotors.insert(port);
}

void Motor::spin(Brain::Port port, float pct) {
    Motor::motors.at(port).move_voltage(12000 * (flippedMotors.contains(port) ? -pct : pct));
};

Piston::Piston(Brain::Port port, bool defState) : Output(port, extend) {
    if (prosPortPortMap.contains(port))
        pistons.insert({port, pros::ADIPneumatics(prosPortPortMap.at(port), defState)});
    else
        Output::~Output(); // Warn
}

void Piston::extend(Brain::Port port, float state) {
    Piston::pistons.at(port).set_value(state);
};