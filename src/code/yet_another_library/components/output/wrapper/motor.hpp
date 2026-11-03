#pragma once

#include "abstract/port.hpp"
#include "pros/motors.hpp"

class ProsMotor : private pros::Motor {
  public:
    using Port = PortInstance::Number;
    ProsMotor(Port port);
    float getVelocity() const;
    void setVoltage(float pct) const;

  private:
    constexpr uint16_t getGearing() const;
};
