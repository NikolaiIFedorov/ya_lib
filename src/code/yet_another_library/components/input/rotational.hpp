#pragma once

#include "../output/abstract/portDesc.hpp"
#include "pros/rotation.hpp"

class Rotational {
  public:
    using Port = PortDesc::Number;
    Rotational(Port port);

    float getAngle(bool continous = true) const;
    float getVelocity() const;
    void reset();

  private:
    pros::Rotation rotational;
};