#include "code/bot.hpp"
#include "code/bot/controller.hpp"
#include "code/bot/output.hpp"

void Bot::init() {
    const auto dt = Bot::addSystem(
        Bot::System<float>{
            {{{Motor{Brain::Port::_1}},
              {{{Controller::InputCmd::A, []() { return 1; }}}, [](float arg) { return 1; }}}}});

    dt(1);
}