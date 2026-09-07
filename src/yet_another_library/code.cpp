#include "code/bot.hpp"
#include "code/bot/controller.hpp"
#include "code/bot/output.hpp"

void Bot::init() {
    const Bot::Outputs left = {Motor{Brain::Port::_1}, Motor{Brain::Port::_3}};
    const Bot::Outputs right = {Motor{Brain::Port::_9}, Motor{Brain::Port::_10}};

    const auto dt = Bot::addSystem(
        Bot::System{
            {{left, {{{Controller::Event::Ly, 1}}, {}}},
             {right, {{{Controller::Event::Ry, 1}}, {}}}}});

    const Bot::Outputs dr4b = {Motor{Brain::Port::_19}};
    static bool clawState = true;
    const Bot::Outputs claw = {Piston{Brain::Port::A, clawState}};

    const auto lift = Bot::addSystem(
        Bot::System{
            {{dr4b, {{{Controller::Event::Ru, 1}, {Controller::Event::Rd, -1}}, {}}},
             {claw,
              {{
                   {Controller::Event::A,
                    []() {
                        clawState = !clawState;
                        return clawState;
                    }},
                   {Controller::Event::Left,
                    []() {
                        clawState = false;
                        return clawState;
                    }},
                   {Controller::Event::Right,
                    []() {
                        clawState = true;
                        return clawState;
                    }},
               },
               {}}}}});
}
