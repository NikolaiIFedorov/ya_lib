#pragma once

#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>
#include <map>

class Controller {
    friend class Bot;

  public:
    enum class InputCmd {
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
    };

    static void attachCallbackToInput(InputCmd inputCmd, std::function<void()>);
    static float getInput(InputCmd);

  private:
    static pros::Controller controller;

    static inline const std::map<InputCmd, pros::controller_digital_e_t> prosButtonInputCmdMap;
    static inline const std::map<InputCmd, pros::controller_analog_e_t> prosAnalogInputCmdMap;

    static std::map<InputCmd, std::vector<std::function<void()>>> inputCmdCallbacksMap;

    static void pollInputs();
};