#include "controller.hpp"
#include "../utils.hpp"
#include "pros/misc.h"

pros::Controller Controller::controller(pros::controller_id_e_t::E_CONTROLLER_MASTER);
std::map<Controller::Event, std::vector<Controller::Callback>> Controller::callbackFromEvent = {};

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
            if (prosButtonFromEvent.contains(event)) {
                if (controller.get_digital_new_press(prosButtonFromEvent.at(event)) ||
                    controller.get_digital_new_release(prosButtonFromEvent.at(event))) {
                    for (const auto &callback : callbacks)
                        callback();
                }
            }

            if (prosAnalogFromEvent.contains(event) &&
                controller.get_analog(prosAnalogFromEvent.at(event))) {
                for (const auto &callback : callbacks)
                    callback();
            }
        }
    });
};

void Controller::addCallback(Event event, Callback callback) {
    callbackFromEvent[event].push_back(std::move(callback));
};

float Controller::getInput(Event event) {
    if (prosButtonFromEvent.contains(event))
        return controller.get_digital(prosButtonFromEvent.at(event));
    else if (prosAnalogFromEvent.contains(event))
        return controller.get_analog(prosAnalogFromEvent.at(event));

    return 0;
}
