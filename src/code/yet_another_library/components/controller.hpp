#pragma once

#include "abstract/event.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <cstdint>
#include <functional>
#include <map>

class Controller {
    friend class ya_lib;

  public:
    static void addEvent(Event::Id event, std::function<void()> callback);

    static std::vector<std::function<void()>> getAutons();
    static void setAuton(std::function<void()> auton);

  private:
    inline static pros::Controller controller{pros::controller_id_e_t::E_CONTROLLER_MASTER};

    enum class EventKind {
        Button,
        Analog,
        Time,
    };
    static EventKind getEventKind(Event::Id event);

    inline static const std::map<Event::Id, pros::controller_digital_e_t> buttonEvents{
        {Event::Id::Left, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_LEFT},
        {Event::Id::Right, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_RIGHT},
        {Event::Id::Up, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_UP},
        {Event::Id::Down, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_DOWN},
        {Event::Id::Lu, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_L1},
        {Event::Id::Ld, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_L2},
        {Event::Id::Ru, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_R1},
        {Event::Id::Rd, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_R2},
        {Event::Id::Y, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_Y},
        {Event::Id::X, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_X},
        {Event::Id::A, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_A},
        {Event::Id::B, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_B},
    };

    inline static const std::map<Event::Id, pros::controller_analog_e_t> analogEvents{
        {Event::Id::Lx, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_LEFT_X},
        {Event::Id::Ly, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_LEFT_Y},
        {Event::Id::Rx, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_RIGHT_X},
        {Event::Id::Ry, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_RIGHT_Y},

    };

    inline static const std::map<Event::Id, std::function<bool()>> isTimeEvents{
        {Event::Id::CompInit, []() { return pros::competition::is_connected(); }},
        {Event::Id::Disabled, []() { return pros::competition::is_disabled(); }},
        {Event::Id::Auton, []() { return pros::competition::is_autonomous(); }},

    };

  public:
    static float getEventVal(Event::Id event);

  private:
    inline static std::map<Event::Id, int8_t> analogVals{
        {Event::Id::Lx, 0},
        {Event::Id::Ly, 0},
        {Event::Id::Rx, 0},
        {Event::Id::Ry, 0},
    };

    inline static std::map<Event::Id, std::vector<std::function<void()>>> callbackEvents;

    static void executeEvents(Event::Id event);
    static void executeEvents();
};
