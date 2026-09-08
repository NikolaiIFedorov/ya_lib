#pragma once

#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>
#include <map>
#include <vector>

class Controller {
    friend class Bot;

  public:
    using Callback = std::function<void()>;

    enum class Event {
        A,
        B,
        X,
        Y,
        Left,
        Up,
        Down,
        Right,
        Lu,
        Ld,
        Ru,
        Rd,
        Ly,
        Lx,
        Ry,
        Rx,
        Auton,
        Init,
        Comp_Init,
        Driver_Control,
    };

    static void addCallback(Event event, Callback callback);
    static float getInput(Event event);

  private:
    static pros::Controller controller;

    static inline const std::map<Event, pros::controller_digital_e_t> prosButtonFromEvent = {
        {Event::A, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_A},
        {Event::B, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_B},
        {Event::X, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_X},
        {Event::Y, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_Y},
        {Event::Left, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_LEFT},
        {Event::Up, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_UP},
        {Event::Right, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_RIGHT},
        {Event::Down, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_DOWN},
        {Event::Lu, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_L1},
        {Event::Ld, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_L2},
        {Event::Ru, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_R1},
        {Event::Rd, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_R2},
    };
    static inline const std::map<Event, pros::controller_analog_e_t> prosAnalogFromEvent = {
        {Event::Ly, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_LEFT_Y},
        {Event::Lx, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_LEFT_X},
        {Event::Ry, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_RIGHT_Y},
        {Event::Rx, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_RIGHT_X}};

    static std::map<Event, std::vector<Callback>> callbackFromEvent;

    static void triggerCallback(Event event);
    static void pollInputs();
};