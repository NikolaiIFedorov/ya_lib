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
        using EquationFunction::EquationFunction;

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

        void operator()(float arg) const;
        float getState() const;

        void addEvent(Controller::Event event, Equation equation) const;
    };

    template <typename... Args> using MacroFunction = std::function<float(Args...)>;
    template <typename... Args> struct Macro : public MacroFunction<Args...> { // Add odom
      public:
        using MacroFunction<Args...>::MacroFunction;

        template <typename Function>
            requires std::is_invocable_r_v<float, Function, Args...>
        Macro(Function function) : MacroFunction<Args...>(function), enabled(true){};
        Macro() : MacroFunction<Args...>([](Args...) { return 0.0f; }), enabled(false) {};

        bool enabled;
    };

    template <typename... MacroArgs>
    using EquationsPair = std::pair<std::map<Controller::Event, Equation>, Macro<MacroArgs...>>;
    template <typename... MacroArgs> struct Equations : public EquationsPair<MacroArgs...> {
      public:
        using EquationsPair<MacroArgs...>::EquationsPair;

        Equations(std::map<Controller::Event, Equation> equations)
            : Equations({equations, Macro<MacroArgs...>()}) {};
        Equations(std::vector<Controller::Event> events) : Equations(convertToMap(events)) {};
        Equations(Controller::Event event) : Equations(std::vector<Controller::Event>{event}) {};

        Equations(std::vector<Controller::Event> events, Macro<MacroArgs...> macro)
            : Equations({convertToMap(events), macro}) {};
        Equations(Macro<MacroArgs...> macro) : Equations({}, macro) {};

      private:
        std::map<Controller::Event, Equation> convertToMap(std::vector<Controller::Event> events) {
            std::map<Controller::Event, Equation> equations;
            for (const auto &event : events)
                equations.insert({event, {}});

            return equations;
        };
    };

    template <typename... MacroArgs> using SystemMap = std::map<Outputs, Equations<MacroArgs...>>;
    template <typename... MacroArgs> class System : public SystemMap<MacroArgs...> {
      public:
        void operator()(MacroArgs... macroArgs) const {
            const auto &subsystems = *this;
            for (const auto &subsystem : subsystems) {
                const auto &macro = subsystem.second.second;
                if (!macro.enabled)
                    continue;

                const auto arg = macro(macroArgs...);
                const auto &outputs = subsystem.first;
                for (const auto &output : outputs)
                    output(arg);
            };
        };

        void addOutputs(const Outputs &outputs, const Equations<MacroArgs...> &equations) {
            const auto &eventEquations = equations.first;
            for (const auto &[event, equation] : eventEquations)
                outputs.addEvent(event, equation);
        };
    };

    static void init();

    template <typename... MacroArgs>
    static System<MacroArgs...> addSystem(System<MacroArgs...> system) {
        for (const auto &[outputs, equations] : system)
            system.addOutputs(outputs, equations);

        return system;
    };

    static void _compInit(); // Add auton selection
    static void _auton();
    static void _drivercontrol();
};
