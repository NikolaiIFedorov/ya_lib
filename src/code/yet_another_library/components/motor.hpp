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
    Motor(Brain::Port port, std::initializer_list<EventDesc> events);
    ~Motor();

    void spin(const float pct) const;
    float getPct() const;

  private:
    ProsMotor motor;

    const Brain::Port port;
    inline static std::map<Brain::Port, Motor *> portMotors;

    float targetPct;
    float integralPct;

    static float mfi(const Motor &motor, float pct, float acceleration);
    static float pid(Motor &motor, float pct);
    static void accelerate(Motor &motor);
    static void staticSpin(const Brain::Port port, const float pct);

    static float staticGetPct(const Brain::Port port);
};