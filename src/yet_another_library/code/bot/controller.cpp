#include "controller.hpp"
#include "../utils.hpp"
#include "pros/misc.h"

pros::Controller Controller::controller(pros::controller_id_e_t::E_CONTROLLER_MASTER);
std::map<Controller::Event, std::vector<Controller::Callback>> Controller::callbackFromEvent;
std::map<Controller::Event, float> Controller::lastAxisVal =
    {{Event::Ly, 0}, {Event::Lx, 0}, {Event::Ry, 0}, {Event::Rx, 0}};

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
    // TRACE([]() {
    for (auto &[event, callbacks] : callbackFromEvent) {
        if (prosButtonFromEvent.contains(event)) {
            if (controller.get_digital_new_press(prosButtonFromEvent.at(event)) ||
                controller.get_digital_new_release(prosButtonFromEvent.at(event))) {
                for (const auto &callback : callbacks)
                    callback();
            }
        }

        if (prosAnalogFromEvent.contains(event) && axisValChanged(event)) {
            for (const auto &callback : callbacks)
                callback();
        }
    }
    // });
};

void Controller::addCallback(Event event, Callback callback) {
    callbackFromEvent[event].push_back(std::move(callback));
};

bool Controller::axisValChanged(Event event) {
    float val = controller.get_analog(prosAnalogFromEvent.at(event));
    if (lastAxisVal[event] == val)
        return false;

    lastAxisVal[event] = val;
    return true;
};

float Controller::getInput(Event event) {
    if (prosButtonFromEvent.contains(event))
        return controller.get_digital(prosButtonFromEvent.at(event));
    else if (prosAnalogFromEvent.contains(event))
        return float(controller.get_analog(prosAnalogFromEvent.at(event))) / 127;

    return 0;
}
