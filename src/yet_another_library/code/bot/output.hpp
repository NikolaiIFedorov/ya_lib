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
    static inline const std::map<Brain::Port, int8_t> prosPortFromPort = {
        {Brain::Port::_1, 1},   {Brain::Port::_2, 2},   {Brain::Port::_3, 3},
        {Brain::Port::_4, 4},   {Brain::Port::_5, 5},   {Brain::Port::_6, 6},
        {Brain::Port::_7, 7},   {Brain::Port::_8, 8},   {Brain::Port::_9, 9},
        {Brain::Port::_10, 10}, {Brain::Port::_11, 11}, {Brain::Port::_12, 12},
        {Brain::Port::_13, 13}, {Brain::Port::_14, 14}, {Brain::Port::_15, 15},
        {Brain::Port::_16, 16}, {Brain::Port::_17, 17}, {Brain::Port::_18, 18},
        {Brain::Port::_19, 19}, {Brain::Port::_20, 20},

    };
    static std::map<Brain::Port, pros::Motor> motors;

    static void spin(Brain::Port port, float pct);
    static float getVoltage(Brain::Port port);
};

class Piston : public Output {
    friend class Output;

  public:
    Piston(Brain::Port port, bool defState, bool flipped = false);

  private:
    static inline const std::map<Brain::Port, uint8_t> prosPortFromPort{
        {Brain::Port::A, 'A'},
        {Brain::Port::B, 'B'},
        {Brain::Port::C, 'C'},
        {Brain::Port::D, 'D'},
        {Brain::Port::E, 'E'},
        {Brain::Port::F, 'F'},
        {Brain::Port::G, 'G'},
        {Brain::Port::H, 'H'},
    };
    static std::map<Brain::Port, pros::adi::Pneumatics> pistons;

    static void extend(Brain::Port port, float state);
    static float getExtended(Brain::Port port);
};