#include "controller.hpp"

void Controller::triggerCallback(Event event) {
    if (!callbackFromEvent.contains(event))
        return;

    const auto &callbacks = callbackFromEvent[event];
    for (const auto &callback : callbacks)
        callback();

    callbackFromEvent.erase(event);
};

void Controller::pollInputs() {

    for (auto &[event, callbacks] : callbackFromEvent) {
        if (prosButtonFromEvent.contains(event) &&
            controller.get_digital(prosButtonFromEvent.at(event))) {
            for (const auto &callback : callbacks)
                callback();
        }

        if (prosAnalogFromEvent.contains(event) &&
            controller.get_analog(prosAnalogFromEvent.at(event))) {
            for (const auto &callback : callbacks)
                callback();
        }
    }
};

void Controller::addCallback(Event event, std::function<void()> callback) {
    callbackFromEvent[event].push_back(callback);
};