#pragma once

#include "abstract/port.hpp"
#include "pros/rotation.hpp"

class Rotational {
  public:
    using Port = PortInstance::Number;
    Rotational(Port port);

    float getAngle(bool continous = true) const;
    float getVelocity() const;
    void reset();

  private:
    pros::Rotation rotational;
};