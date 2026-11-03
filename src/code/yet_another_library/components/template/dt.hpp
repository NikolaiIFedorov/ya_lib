#pragma once

#include "abstract/subSystem.hpp"
#include "inertial.hpp"
#include "motor.hpp"
#include "rotational.hpp"

class Dt : SubSystem<float, float> {
  public:
    enum struct ControlKind { Tank, Arcade };
    struct Config {
        ControlKind controlKind = ControlKind::Arcade;
        Compensation::Config compensationConfig;
    };
    Config config;

    struct Odom {
        Rotational rotational;
        float axisOffset;
    };

    Dt(Motor left, Motor right, Config config);

    void setOdoms(Odom x, Odom y);
    void setInertial(Inertial inertial);

    void operator()(float x, float y, float a);
    void operator()(float l, float r);

  private:
    std::optional<Odom> xOdom = std::nullopt;
    std::optional<Odom> yOdom = std::nullopt;
    std::optional<Inertial> inertial = std::nullopt;
};