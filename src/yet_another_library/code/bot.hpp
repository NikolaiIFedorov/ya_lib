#pragma once

#include "bot/controller.hpp"
#include "bot/output.hpp"
#include "utils.hpp"
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

    template <typename... MacroArgs> using SubMacroFunction = std::function<float(MacroArgs...)>;
    template <typename... Args> struct SubMacro : public SubMacroFunction<Args...> {
      public:
        using SubMacroFunction<Args...>::SubMacroFunction;

        template <typename Function>
            requires std::is_invocable_r_v<float, Function, Args...>
        SubMacro(Function function) : SubMacroFunction<Args...>(function), enabled(true){};
        SubMacro() : SubMacroFunction<Args...>([](Args...) { return 0.0f; }), enabled(false) {};

        bool enabled;
    };

  private:
    template <typename... MacroArgs>
    using EquationsPair = std::pair<std::map<Controller::Event, Equation>, SubMacro<MacroArgs...>>;
    template <typename... MacroArgs> struct Equations : public EquationsPair<MacroArgs...> {
      public:
        using EquationsPair<MacroArgs...>::EquationsPair;

        Equations(std::map<Controller::Event, Equation> equations)
            : Equations({equations, SubMacro<MacroArgs...>()}) {};
        Equations(std::vector<Controller::Event> events) : Equations(convertToMap(events)) {};
        Equations(Controller::Event event) : Equations(std::vector<Controller::Event>{event}) {};

        Equations(std::vector<Controller::Event> events, SubMacro<MacroArgs...> macro)
            : Equations({convertToMap(events), macro}) {};
        Equations(SubMacro<MacroArgs...> macro) : Equations({}, macro) {};

      private:
        std::map<Controller::Event, Equation> convertToMap(std::vector<Controller::Event> events) {
            return TRACE([events]() {
                std::map<Controller::Event, Equation> equations;
                for (const auto &event : events)
                    equations.insert({event, {}});

                return equations;
            });
        };
    };

  public:
    using OutputVector = std::vector<Output>;
    struct Outputs : public OutputVector {
      public:
        using OutputVector::OutputVector;

        Outputs(Output output);

        void operator()(float arg) const;
        float getState() const;

        void addEvent(Controller::Event event, Equation equation) const;
    };

    template <typename... MacroArgs> using SystemMap = std::map<Outputs, Equations<MacroArgs...>>;
    template <typename... MacroArgs> class System : public SystemMap<MacroArgs...> {
      public:
        void operator()(MacroArgs... macroArgs) const {
            TRACE([this, macroArgs...]() {
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
            });
        };

        Outputs addOutputs(const Outputs &outputs, const Equations<MacroArgs...> &equations) const {
            return TRACE([equations, outputs]() {
                const auto &eventEquations = equations.first;
                for (const auto &[event, equation] : eventEquations)
                    outputs.addEvent(event, equation);

                return outputs;
            });
        };
    };

    static void init();

    template <typename... MacroArgs>
    static System<MacroArgs...> addSystem(System<MacroArgs...> system) {
        return TRACE([system]() {
            for (const auto &[outputs, equations] : system)
                system.addOutputs(outputs, equations);

            return system;
        });
    };

    static void _compInit();
    static void _auton();
    static void _drivercontrol();

  private:
    static Controller::Callback getAuton();
};
