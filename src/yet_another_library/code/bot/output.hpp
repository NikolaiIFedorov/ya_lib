#pragma once

#include "brain.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include <cstdint>
#include <functional>
#include <map>
#include <unordered_set>

class Output {
    friend class Motor;
    friend class Piston;

  public:
    using Call = std::function<void(Brain::Port, float)>;
    using GetState = std::function<float(Brain::Port)>;

    Output(Brain::Port, bool flipped = false);
    Output(bool initExtended, Brain::Port, bool flipped = false);

    bool operator<(const Output &compared) const {
        return port < compared.port;
    }

    float getState() const;
    void operator()(float arg) const;

  private:
    Output(Brain::Port port, Call call, GetState getState);

    Brain::Port port;

    Call _call;
    GetState _getState;

    static std::unordered_set<Brain::Port> flippedOutputs;
};

class Motor : public Output {
    friend class Output;

  public:
    Motor(Brain::Port port, bool flipped = false);

  private:
    static inline const std::map<Brain::Port, int8_t> prosPortFromPort;
    static std::map<Brain::Port, pros::Motor> motors;

    static void spin(Brain::Port port, float pct);
    static float getVoltage(Brain::Port port);
};

class Piston : public Output {
    friend class Output;

  public:
    Piston(Brain::Port port, bool defState, bool flipped = false);

  private:
    static inline const std::map<Brain::Port, uint8_t> prosPortFromPort;
    static std::map<Brain::Port, pros::ADIPneumatics> pistons;

    static void extend(Brain::Port port, float state);
    static float getExtended(Brain::Port port);
};