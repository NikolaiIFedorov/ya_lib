#include "code/bot.hpp"
#include "code/bot/controller.hpp"

// Add logging
void Bot::init() {
    const Bot::Outputs left = {{Brain::Port::_1}, {Brain::Port::_3}};
    const Bot::Outputs right = {{Brain::Port::_9}, {Brain::Port::_10}};
    const System dt = Bot::addSystem(
        {{{left,
           {
               {
                   {Controller::Event::Ly, 1},
               },
           }},
          {right,
           {
               {
                   {Controller::Event::Ry, 1},
               },
           }}}});

    const Bot::Outputs dr4b = {Brain::Port::_19};
    const Bot::Outputs claw = {true, Brain::Port::A};
    const System lift = Bot::addSystem(
        {{{dr4b,
           {
               {
                   {Controller::Event::Ru, 1},
                   {Controller::Event::Rd, -1},
               },
           }},
          {claw,
           {
               {
                   {Controller::Event::A, !claw.getState()},
                   {Controller::Event::Left, false},
                   {Controller::Event::Right, true},
               },

           }}}});
}
