#pragma once

#include "brain.hpp"
#include "pros/imu.hpp"

class Inertial {
  public:
    Inertial(Brain::Port port);

    float getRotation();

  private:
    pros::Imu inertial;
};