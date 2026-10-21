#include "eventPort.hpp"
#include "controller.hpp"

EventPort::EventPort(
    std::initializer_list<EventDesc> events,
    std::function<void(float)> write,
    std::function<float()> read)
    : write(write), read(read) {
    for (const auto &[event, arg] : events)
        Controller::addEvent(event, [write, arg]() { write(arg()); });
};