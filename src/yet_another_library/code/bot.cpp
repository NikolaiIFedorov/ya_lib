#include "bot.hpp"
#include "bot/controller.hpp"

template <typename... MacroArgs>
Bot::System<MacroArgs...> Bot::addSystem(System<MacroArgs...> system) {
    for (const auto &[outputs, input] : system) {
        const auto &inputCmdEquations = input.first;
        for (const auto &[inputCmd, equation] : inputCmdEquations) {
            for (const auto &output : outputs)
                Controller::attachCallbackToInput(inputCmd, [output, equation]() {
                    output(equation());
                });
        }
    }

    return system;
};

template <typename... MacroArgs>
Bot::System<MacroArgs...>::System(Subsystems subsystems) : Subsystems(subsystems){};
template <typename... MacroArgs>
void Bot::System<MacroArgs...>::operator()(MacroArgs... macroArgs) const {
    const auto &subsystems = *this;
    for (const auto &subsystem : subsystems) {
        const auto &arg = subsystem.second.second(macroArgs...);
        const auto &outputs = subsystem.first;
        for (const auto &output : outputs)
            output(arg);
    };
};