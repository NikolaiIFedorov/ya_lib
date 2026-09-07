#pragma once

#include "bot/controller.hpp"
#include "bot/output.hpp"
#include <functional>
#include <map>
#include <vector>

class Bot {
  public:
    using EquationFunction = std::function<float()>;
    struct Equation : public EquationFunction {
      public:
        template <typename Function>
            requires std::is_invocable_r_v<float, Function>
        Equation(Function function) : EquationFunction(function), constant(false){};
        Equation(float val = 1);

        bool constant;
    };

    using OutputVector = std::vector<Output>;
    struct Outputs : public OutputVector {
      public:
        using OutputVector::OutputVector;
        Outputs(Output output);
    };

    template <typename... Args> using MacroFunction = std::function<float(Args...)>;
    template <typename... Args> struct Macro : public MacroFunction<Args...> {
      public:
        template <typename Function>
            requires std::is_invocable_r_v<float, Function, Args...>
        Macro(Function function) : MacroFunction<Args...>(function), enabled(true){};
        Macro() : MacroFunction<Args...>([](Args...) { return 0.0f; }), enabled(false) {};

        bool enabled;
    };

    template <typename... MacroArgs>
    using Subsystems =
        std::map<Outputs, std::pair<std::map<Controller::Event, Equation>, Macro<MacroArgs...>>>;
    template <typename... MacroArgs> class System : public Subsystems<MacroArgs...> {
      public:
        using Subsystems = Bot::Subsystems<MacroArgs...>;

        System(Subsystems subsystems) : Subsystems(subsystems) {};
        System(
            Outputs outputs,
            std::map<Controller::Event, Equation> inputs,
            Macro<MacroArgs...> macro)
            : System({outputs, {inputs, macro}}) {};
        System(
            Outputs outputs,
            Controller::Event input,
            Equation inputEquation,
            Macro<MacroArgs...> macro)
            : System(outputs, {input, inputEquation}, macro) {};

        void operator()(MacroArgs... macroArgs) const {
            const auto &subsystems = *this;
            for (const auto &subsystem : subsystems) {
                const auto &macro = subsystem.second.second;
                if (!macro.enabled)
                    continue;

                const auto &arg = macro(macroArgs...);
                const auto &outputs = subsystem.first;
                for (const auto &output : outputs)
                    output(arg);
            };
        };
    };

    static void init();

    template <typename... MacroArgs>
    static System<MacroArgs...> addSystem(System<MacroArgs...> system) {
        for (const auto &[outputs, input] : system) {
            const auto &eventEquations = input.first;
            for (const auto &[event, equation] : eventEquations) {
                for (const auto &output : outputs)
                    Controller::attachCallbackToInput(event, [output, equation]() {
                        output(equation());
                    });
            }
        }

        return system;
    };

    static void _compInit();
    static void _auton();
    static void _drivercontrol();
};
