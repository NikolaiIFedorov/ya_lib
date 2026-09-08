#include "controller.hpp"
#include "../utils.hpp"
#include "pros/misc.h"

pros::Controller Controller::controller(pros::controller_id_e_t::E_CONTROLLER_MASTER);
std::map<Controller::Event, std::vector<std::function<void()>>> Controller::callbackFromEvent = {};

void Controller::triggerCallback(Event event) {
    TRACE([event]() {
        if (!callbackFromEvent.contains(event))
            return;

        const auto &callbacks = callbackFromEvent[event];
        for (const auto &callback : callbacks)
            callback();

        callbackFromEvent.erase(event);
    });
};

void Controller::pollInputs() {
    TRACE([]() {
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
    });
};

void Controller::addCallback(Event event, std::function<void()> callback) {
    callbackFromEvent[event].push_back(callback);
};