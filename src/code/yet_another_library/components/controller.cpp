#include "controller.hpp"
#include "../utils/log.hpp"

void Controller::addEvent(Event::Id event, std::function<void()> callback) {
    callbackEvents[event].push_back(callback);
};

std::vector<std::function<void()>> Controller::getAutons() {
    return callbackEvents.at(Event::Id::Auton);
};

void Controller::setAuton(std::function<void()> auton) {
    callbackEvents[Event::Id::Auton] = {auton};
};
Controller::EventKind Controller::getEventKind(Event event) {
    const auto id = event.id;
    switch (id) {
    case Event::Id::Left:
    case Event::Id::Right:
    case Event::Id::Up:
    case Event::Id::Down:
    case Event::Id::Lu:
    case Event::Id::Ld:
    case Event::Id::Ru:
    case Event::Id::Rd:
    case Event::Id::Y:
    case Event::Id::X:
    case Event::Id::A:
    case Event::Id::B:
        return EventKind::Button;
    case Event::Id::Lx:
    case Event::Id::Ly:
    case Event::Id::Rx:
    case Event::Id::Ry:
        return EventKind::Analog;
    case Event::Id::CompInit:
    case Event::Id::Disabled:
    case Event::Id::Auton:
    case Event::Id::DriverControl:
        return EventKind::Time;
    };

    return EventKind::Time;
}
float Controller::getEventVal(Event event) {
    const auto id = event.id;
    const auto eventKind = getEventKind(event);
    switch (eventKind) {
    case EventKind::Button:
        return controller.get_digital(buttonEvents.at(id));
    case EventKind::Analog:
        return controller.get_analog(analogEvents.at(id)) / 127.0f;
    case EventKind::Time:
        return isTimeEvents.at(id)();
    };
};

void Controller::executeEvents(Event event) {
    Log::Trace([event] {
        const auto id = event.id;
        if (!callbackEvents.contains(id))
            return;

        const auto &callbacks = callbackEvents[id];
        for (const auto &callback : callbacks)
            callback();

        if (getEventKind(id) == EventKind::Time)
            callbackEvents.erase(id);
    });
}
void Controller::executeEvents() {
    Log::Trace([] {
        for (const auto &callbacksEvents : callbackEvents) {
            const auto event = callbacksEvents.first;
            const auto eventKind = getEventKind(event);
            switch (eventKind) {
            case EventKind::Button: {
                const auto button = buttonEvents.at(event);
                if (controller.get_digital_new_press(button) ||
                    controller.get_digital_new_release(button)) {
                    executeEvents(event);
                }
            } break;
            case EventKind::Analog: {
                auto &val = analogVals[event];
                const auto controllerVal = controller.get_analog(analogEvents.at(event));
                if (val != controllerVal) {
                    val = controllerVal;
                    executeEvents(event);
                }
            } break;
            case EventKind::Time:
                continue;
            };
        }
    });
}
