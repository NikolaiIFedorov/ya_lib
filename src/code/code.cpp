#include "yet_another_library/bot.hpp"

void Bot::init() {
    auto test = addSystem<>([](auto &system) {
        const auto instance = addPortInstance({Brain::Port::_1});
        addEvent(instance, Controller::Event::A, []() {
            return Controller::getEventVal(Controller::Event::A);
        });
    });
};
