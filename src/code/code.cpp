#include "yet_another_library/bot.hpp"

void Bot::init() {
    System<> system([system]() {
        PortInstance test(Brain::Port::_1);
        test.addEvent(Controller::Event::A, []() { return 1.0f; });
        system.addSubMacro(test, []() { return 1.0f; });
    });
};
