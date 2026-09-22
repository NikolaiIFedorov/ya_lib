#pragma once

#include "pros/misc.h"
#include "pros/misc.hpp"
#include <cstdint>
#include <functional>
#include <map>
class Controller {
    friend class _ya_lib;

  public:
    enum class Event {
        Left,
        Right,
        Up,
        Down,
        Lu,
        Ld,
        Ru,
        Rd,
        Y,
        X,
        A,
        B,
        Lx,
        Ly,
        Rx,
        Ry,
        CompInit,
        Disabled,
        Auton,
        DriverControl,
    };

    static void addEvent(Event event, std::function<void()> callback) {
        callbackEvents[event].push_back(callback);
    };

  private:
    inline static pros::Controller controller{pros::controller_id_e_t::E_CONTROLLER_MASTER};

    enum class EventKind {
        Button,
        Analog,
        Time,
    };
    static EventKind getEventKind(Event event) {
        switch (event) {
        case Event::Left:
        case Event::Right:
        case Event::Up:
        case Event::Down:
        case Event::Lu:
        case Event::Ld:
        case Event::Ru:
        case Event::Rd:
        case Event::Y:
        case Event::X:
        case Event::A:
        case Event::B:
            return EventKind::Button;
        case Event::Lx:
        case Event::Ly:
        case Event::Rx:
        case Event::Ry:
            return EventKind::Analog;
        case Event::CompInit:
        case Event::Disabled:
        case Event::Auton:
        case Event::DriverControl:
            return EventKind::Time;
        };
    }

    inline static const std::map<Event, pros::controller_digital_e_t> buttonEvents{
        {Event::Left, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_LEFT},
        {Event::Right, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_RIGHT},
        {Event::Up, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_UP},
        {Event::Down, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_DOWN},
        {Event::Lu, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_L1},
        {Event::Ld, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_L2},
        {Event::Ru, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_R1},
        {Event::Rd, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_R2},
        {Event::Y, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_Y},
        {Event::X, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_X},
        {Event::A, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_A},
        {Event::B, pros::controller_digital_e_t::E_CONTROLLER_DIGITAL_B},
    };

    inline static const std::map<Event, pros::controller_analog_e_t> analogEvents{
        {Event::Lx, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_LEFT_X},
        {Event::Ly, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_LEFT_Y},
        {Event::Rx, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_RIGHT_X},
        {Event::Ry, pros::controller_analog_e_t::E_CONTROLLER_ANALOG_RIGHT_Y},

    };

    inline static std::map<Event, int8_t> analogVals{
        {Event::Lx, 0},
        {Event::Ly, 0},
        {Event::Rx, 0},
        {Event::Ry, 0},
    };

    inline static std::map<Event, std::vector<std::function<void()>>> callbackEvents;

    static void executeEvents(Event event) {
        if (!callbackEvents.contains(event))
            return;

        const auto &callbacks = callbackEvents[event];
        for (const auto &callback : callbacks)
            callback();

        if (getEventKind(event) == EventKind::Time)
            callbackEvents.erase(event);
    }
    static void executeEvents() {
        for (const auto &callbacksEvents : callbackEvents) {
            const auto event = callbacksEvents.first;
            const auto eventKind = getEventKind(event);
            switch (eventKind) {
            case EventKind::Button: {
                const auto button = buttonEvents.at(event);
                if (controller.get_digital_new_press(button) ||
                    controller.get_digital_new_release(button))
                    executeEvents(event);
            } break;
            case EventKind::Analog: {
                auto &val = analogVals[event];
                const auto controllerVal = controller.get_analog(analogEvents.at(event));
                if (val != controllerVal) {
                    val = controllerVal;
                    executeEvents(event);
                }
            } break;
            case EventKind::Time:
                continue;
            };
        }
    }
};