#include "code/bot.hpp"
#include "code/bot/controller.hpp"
#include "code/utils.hpp"

void Bot::init() {
    TRACE([]() {
        const Outputs motor = Motor{Brain::Port::_1};
        motor.addEvent(Controller::Event::Rd, -1);
        motor.addEvent(Controller::Event::Ru, 1);

        System system;
        addSystem(system);
    });
}