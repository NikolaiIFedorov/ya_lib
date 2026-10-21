#include "motor.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/rtos.hpp"

ProsMotor::ProsMotor(Brain::Port port) : pros::Motor(static_cast<uint8_t>(port)) {}
constexpr uint16_t ProsMotor::getGearing() const {
    switch (this->get_gearing()) {
    case pros::MotorGears::rpm_100:
        return 100;
    case pros::MotorGears::rpm_200:
        return 200;
    case pros::MotorGears::rpm_600:
        return 600;
    default:
        return 200;
    }
};
float ProsMotor::getPct() const {
    return this->get_actual_velocity() / getGearing();
};

Motor::Motor(Brain::Port port, std::initializer_list<EventDesc> events)
    : EventPort(
          events,
          [port](float pct) { staticSpin(port, pct); },
          [port]() { return staticGetPct(port); }),
      motor(port), port(port) {
    portMotors[port] = this;
    for (const auto &[event, pct] : events)
        Controller::addEvent(event, [port = this->port, pct]() { staticSpin(port, pct()); });
};
Motor::~Motor() {
    portMotors.erase(port);
};

void Motor::spin(const float pct) const {
    staticSpin(port, pct);
}
float Motor::getPct() const {
    return motor.getPct();
}

float ks;
float kv;
float ka;
float Motor::mfi(const Motor &motor, float pct, float acceleration) {
    return ks * getSign(pct) + kv * pct + ka * acceleration;
};

float tick;
float dt = tick * 0.001;
float kp;
float ki;
float Motor::pid(Motor &motor, float pct) {
    const auto diff = pct - motor.getPct();
    const auto integral = motor.integralPct += diff * dt;

    return kp * diff + ki * integral;
};

float a;
void Motor::accelerate(Motor &motor) {
    const auto pct = std::clamp(motor.targetPct - motor.getPct(), -a, a);
    motor.spin(mfi(motor, pct, a * std::abs(pct) / pct / dt) + pid(motor, pct));
};

void Motor::staticSpin(const Brain::Port port, const float pct) {
    portMotors[port]->targetPct = pct;
    static pros::Task mfiPid{[]() {
        while (true) {
            for (auto &[port, motor] : portMotors)
                accelerate(*motor);

            pros::delay(tick);
        }
    }};
};
float Motor::staticGetPct(const Brain::Port port) {
    return portMotors[port]->getPct();
}
