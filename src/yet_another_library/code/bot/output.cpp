#include "output.hpp"
#include "../utils.hpp"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"
#include <unordered_set>

std::unordered_set<Brain::Port> Output::flippedOutputs;
std::map<Brain::Port, pros::Motor> Motor::motors;
pros::MutexVar<std::map<Brain::Port, float>> Motor::targetVoltageFromMotor;
std::map<Brain::Port, pros::adi::Pneumatics> Piston::pistons;

Output::Output(Brain::Port port, Call call, GetState getState)
    : port(port), _call(std::move(call)), _getState(std::move(getState)) {};

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

// TODO: Add adaptive pid:
// * 1. Impulse
// 2. Motor acceleration
// 3. Inertia
// 4. Acceleration
// 5. Counter current?

void Motor::motorPid() {
    TRACE([]() {
        static bool taskPid = false;
        if (taskPid)
            return;

        taskPid = true;

        std::map<Brain::Port, float> thisTargetVoltageFromMotor = *targetVoltageFromMotor.lock();
        while (!thisTargetVoltageFromMotor.empty()) {
            for (auto it = thisTargetVoltageFromMotor.begin();
                 it != thisTargetVoltageFromMotor.end();) {
                const auto port = it->first;
                const auto targetVoltage = it->second;

                const float currentVoltage = getVoltage(port);
                const float voltageDiff = targetVoltage - currentVoltage;

                if (std::abs(voltageDiff) < 1000.0f) {
                    motors.at(port).move_voltage(targetVoltage);
                    it = thisTargetVoltageFromMotor.erase(it);
                    (*targetVoltageFromMotor.lock()).erase(port);
                    continue;
                }

                motors.at(port).move_voltage(
                    currentVoltage + (voltageDiff > 0 ? 1000.0f : -1000.0f));

                ++it;
            }
            pros::delay(10);
            thisTargetVoltageFromMotor = *targetVoltageFromMotor.lock();
        }

        taskPid = false;
    });
};

void Motor::spin(Brain::Port port, float pct) {
    (*targetVoltageFromMotor.lock())[port] = 12000 * (flippedOutputs.contains(port) ? -pct : pct);

    pros::Task pid(motorPid);
};

float Motor::getVoltage(Brain::Port port) {
    return float(motors.at(port).get_voltage());
};

Piston::Piston(Brain::Port port, bool defState, bool flipped)
    : Output(port, Call(setExtended), GetState(getExtended)) {
    TRACE([port, flipped, defState]() {
        if (prosPortFromPort.contains(port))
            pistons.insert({port, pros::adi::Pneumatics(prosPortFromPort.at(port), defState)});

        if (flipped)
            flippedOutputs.insert(port);
    });
}

void Piston::setExtended(Brain::Port port, float extended) {
    pistons.at(port).set_value(flippedOutputs.contains(port) ? !bool(extended) : extended);
};

float Piston::getExtended(Brain::Port port) {
    return pistons.at(port).is_extended();
};