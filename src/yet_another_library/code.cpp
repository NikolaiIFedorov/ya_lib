#include "code/bot.hpp"
#include "code/bot/controller.hpp"
#include "code/bot/output.hpp"

void Bot::init() {
    const auto dt = Bot::addSystem(
        Bot::System<1>{
            {{{Motor{Brain::Port::_1}},
              {{{Controller::InputCmd::A, []() { return 1; }}},
               [](std::array<float, 1> macroArgs) { return macroArgs[0]; }}}}});

    dt({1});
}