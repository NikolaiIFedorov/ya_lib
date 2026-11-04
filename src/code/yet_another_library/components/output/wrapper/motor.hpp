#pragma once

#include "abstract/portDesc.hpp"
#include "pros/motors.hpp"

class ProsMotor : private pros::Motor {
  public:
    using Port = Flippable<PortDesc::Number>;
    ProsMotor(Port port);
    float getVelocity() const;
    void setVoltage(float pct) const;

  private:
    const bool flipped;
    constexpr uint16_t getGearing() const;
};
