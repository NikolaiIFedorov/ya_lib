#pragma once

#include "brain.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include <cstdint>
#include <functional>
#include <map>
#include <unordered_set>

class Output {
    friend class Bot;

  public:
    Output(Brain::Port port, std::function<void(Brain::Port, float)> call);

    bool operator<(const Output &compared) const {
        return port < compared.port;
    }

  private:
    Brain::Port port;

    void operator()(float arg) const;
};

class Motor : public Output {
  public:
    Motor(Brain::Port port, bool flipped = false);

  private:
    static inline const std::map<Brain::Port, int8_t> prosPortPortMap;
    static std::map<Brain::Port, pros::Motor> motors;
    static std::unordered_set<Brain::Port> flippedMotors;

    static void spin(Brain::Port port, float pct);
};

class Piston : public Output {
  public:
    Piston(Brain::Port port, bool defState = true);

  private:
    static inline const std::map<Brain::Port, uint8_t> prosPortPortMap;
    static std::map<Brain::Port, pros::ADIPneumatics> pistons;

    static void extend(Brain::Port port, float state);
};