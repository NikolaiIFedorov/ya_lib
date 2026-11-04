#pragma once

#include "abstract/portDesc.hpp"
#include "pros/imu.hpp"

class Inertial {
  public:
    using Port = PortDesc::Letter;
    Inertial(Port port);

    float getRotation();

  private:
    pros::Imu inertial;
};