#include "controller.hpp"

void Controller::triggerCallback(Event event) {
    if (!eventCallbacksMap.contains(event))
        return;

    const auto &callbacks = eventCallbacksMap[event];
    for (const auto &callback : callbacks)
        callback();

    eventCallbacksMap.erase(event);
};

void Controller::pollInputs() {
    for (auto &[event, callbacks] : eventCallbacksMap) {
        if (prosButtonEventMap.contains(event) &&
            controller.get_digital(prosButtonEventMap.at(event))) {
            for (const auto &callback : callbacks)
                callback();
        }

        if (prosAnalogEventMap.contains(event) &&
            controller.get_analog(prosAnalogEventMap.at(event))) {
            for (const auto &callback : callbacks)
                callback();
        }
    }
};

void Controller::addCallback(Event event, std::function<void()> callback) {
    eventCallbacksMap[event].push_back(callback);
};