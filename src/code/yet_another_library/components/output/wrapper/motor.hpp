#pragma once

#include "brain.hpp"
#include "pros/motors.hpp"

class ProsMotor : private pros::Motor {
  public:
    ProsMotor(Brain::Port port);
    float getVelocity() const;
    void setVoltage(float pct) const;

  private:
    constexpr uint16_t getGearing() const;
};
