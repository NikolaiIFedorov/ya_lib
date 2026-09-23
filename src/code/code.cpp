#include "yet_another_library/bot.hpp"

void Bot::init() {
    System<> system([](auto &system) {
        PortInstance motor = system.addPortInstance(Brain::Port::_1);
        motor.addEvent(Controller::Event::A, []() {
            return Controller::getEventVal(Controller::Event::A);
        });

        system.addSubMacro(motor, []() { return 1.0f; });
    });
};
