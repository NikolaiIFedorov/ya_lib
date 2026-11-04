#include "motor.hpp"

ProsMotor::ProsMotor(Port port)
    : pros::Motor(static_cast<uint8_t>(port.port)), flipped(port.flipped) {}
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
float ProsMotor::getVelocity() const {
    return this->get_actual_velocity() / getGearing() * (flipped == true ? -1 : 1);
};

void ProsMotor::setVoltage(float pct) const {
    this->move_voltage(pct * 12000 * (flipped == true ? -1 : 1));
};