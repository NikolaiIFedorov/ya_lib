#include "bot.hpp"
#include "abstract/portDesc.hpp"

Motor Bot::addMotor(
    std::vector<PortNumber> ports,
    std::initializer_list<PortDesc::EventDesc> events,
    Motor::Config config) {
    return Motor(ports, events, config);
};

Piston
Bot::addPiston(std::vector<PortLetter> ports, std::initializer_list<PortDesc::EventDesc> events) {
    return Piston(ports, events);
};

void Bot::addEvent(Event event, std::function<void()> callback) {
    event(callback);
}

float Bot::getEventVal(Event event) {
    return event();
};
