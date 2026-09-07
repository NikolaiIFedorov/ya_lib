#include "controller.hpp"

void Controller::pollInputs() {
    for (auto &[inputCmd, callbacks] : inputCmdCallbacksMap) {
        if (prosButtonInputCmdMap.contains(inputCmd) &&
            controller.get_digital(prosButtonInputCmdMap.at(inputCmd))) {
            for (const auto &callback : callbacks)
                callback();
        }

        if (prosAnalogInputCmdMap.contains(inputCmd) &&
            controller.get_analog(prosAnalogInputCmdMap.at(inputCmd))) {
            for (const auto &callback : callbacks)
                callback();
        }
    }
};

void Controller::attachCallbackToInput(InputCmd inputCmd, std::function<void()> callback) {
    inputCmdCallbacksMap[inputCmd].push_back(callback);
};