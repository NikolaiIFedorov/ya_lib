#pragma once

#include "brain.hpp"
#include "eventPort.hpp"
#include "pros/motors.hpp"
#include <initializer_list>

class ProsMotor : private pros::Motor {
  public:
    ProsMotor(Brain::Port port);
    float getPct() const;

  private:
    constexpr uint16_t getGearing() const;
};

class Motor : EventPort {
  public:
    Motor(std::vector<Brain::Port> ports, std::initializer_list<EventDesc> events);
    ~Motor();

    void spin(const float pct);
    float getPct() const;

  private:
    std::vector<ProsMotor> prosMotors;

    const std::vector<Brain::Port> ports;
    inline static std::vector<Motor *> motors;

    float targetPct;
    float integralPct;

    static float mfi(const Motor &motor, float pct, float acceleration);
    static float pid(Motor &motor, float pct);
    static void accelerate(Motor &motor);
    static void staticSpin(Motor *motor, const float pct);
};