#pragma once

#include "brain.hpp"
#include "pros/rotation.hpp"

class Rotational {
  public:
    Rotational(Brain::Port port);

    float getAngle(bool continous = true) const;
    float getVelocity() const;
    void reset();

  private:
    pros::Rotation rotational;
};