#include "motor.hpp"
#include "compensation/compensation.hpp"
#include "pros/rtos.hpp"
#include "ya_lib.hpp"
#include <optional>

Motor::Motor(std::vector<Brain::Port> ports, std::initializer_list<EventDesc> events, Config config)
    : Port(
          events,
          [this](float pct) { spin(pct); },
          [this]() { return getVelocity(); }),
      prosMotors(std::vector<ProsMotor>(ports.begin(), ports.end())),
      compensation(std::nullopt, 1, config) {

    motors.push_back(this);
};

Compensation::Config &Motor::getConfig() const {
    return compensation.config;
};

void Motor::setConfig(Config config) {
    compensation.config = config;
};

void Motor::setRotational(Brain::Port port) {
    rotational.emplace(port);
};

void Motor::spin(const float pct) const {
    staticSpin(this, pct);
}
void Motor::spin(const float pct, float pos) const {
    staticSpin(this, pct, pos);
}
void Motor::setVoltage(const Motor &motor, const float pct) {
    const auto &prosMotors = motor.prosMotors;
    for (const auto &prosMotor : prosMotors)
        prosMotor.setVoltage(pct);
}
float Motor::getVelocity() const {
    float average = 0;
    for (const auto &motor : prosMotors)
        average += motor.getVelocity();

    return average / prosMotors.size();
}

void Motor::staticSpin(
    const Motor *motor,
    float targetVelocity,
    std::optional<float> targetPosition) {
    motor->compensation.setTargetPosition(targetPosition, motor->getVelocity());
    motor->compensation.setTargetVelocity(targetVelocity, motor->getVelocity());
    static pros::Task mfiPid{[]() {
        while (true) {
            for (auto &motor : motors) {
                const auto voltage =
                    motor->compensation.getVoltage(motor->getVelocity(), motor->getVelocity());

                if (!voltage.has_value())
                    continue;

                motor->spin(voltage.value());
            }

            pros::delay(ya_lib::config.tickMs);
        }
    }};
};
