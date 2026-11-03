#include "yet_another_library/bot.hpp"

void Bot::init(Config config) {
    auto dt = addSystem([](auto init) {
        const auto left = addMotor(
            {Port::_1, Port::_2, Port::_3},
            {Event::Ly, [] { return getEventVal(Event::Ly); }},
            {});
        // Inits the left Drivetain motors. It's constructor handles the event handling.

        const auto right = addMotor(
            {Port::_10, Port::_9, Port::_8},
            {Event::Ry, [] { return getEventVal(Event::Ry); }},
            {});
    });

    auto lift = addSystem<float /*The system macro call arguments*/>([](auto init) {
        const auto dr4b = addMotor(
            {Port::_4},
            {{Event::Ru, [] { return getEventVal(Event::Ru); }},
             {Event::Rd, [] { return -getEventVal(Event::Rd); }}},
            {});

        init.addSubMacro(dr4b, [](float height) { return height; });
        // Adds a sub macro to the lift system. A submacro is essentially what the motor should do
        // when the system gets called with it's macro args

        const auto claw = addPiston(
            {Port::A},
            {{Event::Left, [] { return false; }}, {Event::Right, [] { return true; }}});
        init.addSubMacro(claw, [](float height) { return height != 0; });
    });

    addEvent(Event::A, [lift] { lift(0); });
    // Adds an event to the controller which resets the the dr4b height to lowerd.
};
