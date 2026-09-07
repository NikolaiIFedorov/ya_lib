#include "bot.hpp"
#include "bot/controller.hpp"

template <size_t subsystemCount>
const Bot::System<subsystemCount> Bot::addSystem(System<subsystemCount> system) {
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

template <size_t subsystemCount>
Bot::System<subsystemCount>::System(Subsystems subsystems) : Subsystems(subsystems){};
template <size_t subsystemCount>
void Bot::System<subsystemCount>::operator()(std::array<float, subsystemCount> macroArgs) const {
    const auto &subsystems = *this;
    for (const auto &subsystem : subsystems) {
        const auto &arg = subsystem.second.second(macroArgs);
        const auto &outputs = subsystem.first;
        for (const auto &output : outputs)
            output(arg);
    };
};