#include "bot.hpp"

Motor Bot::addMotor(
    std::vector<Brain::Port> ports,
    std::initializer_list<SystemPort::EventDesc> events,
    Motor::Config config) {
    return Motor(ports, events, config);
};

Piston Bot::addPiston(
    std::vector<Brain::Port> ports,
    std::initializer_list<SystemPort::EventDesc> events) {
    return Piston(ports, events);
};

void Bot::addEvent(Event event, std::function<void()> callback) {
    Controller::addEvent(event, callback);
}

float Bot::getEventVal(Event event) {
    return Controller::getEventVal(event);
};
