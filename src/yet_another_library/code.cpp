#include "code/bot.hpp"
#include "code/bot/controller.hpp"
#include "code/utils.hpp"

void Bot::init() {
    TRACE([]() {
        const auto motor = Motor{Brain::Port::_1};
        System system;
        system.addOutputs({motor}, {{{Controller::Event::Rd, -1}, {Controller::Event::Ru, 1}}, {}});
    });
}