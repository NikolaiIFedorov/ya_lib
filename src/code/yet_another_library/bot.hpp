#pragma once

#include "abstract/portDesc.hpp"
#include "abstract/subSystem.hpp"
#include "motor.hpp"
#include "piston.hpp"
#include "ya_lib.hpp"

class Bot {
    using Config = ya_lib::Config &;
    using PortNumber = PortDesc::Number;
    using PortLetter = PortDesc::Letter;

  public:
    static void init(Config config);

  private:
    template <typename... MacroArgs>
    static SubSystem<MacroArgs...> addSystem(SubSystem<MacroArgs...>::Init init) {
        return SubSystem<MacroArgs...>(init);
    };

    static Motor addMotor(
        std::vector<Flippable<PortNumber>> ports,
        std::initializer_list<PortDesc::EventDesc> events,
        Motor::Config config);

    static Piston addPiston(
        std::vector<Flippable<PortLetter>> ports,
        std::initializer_list<PortDesc::EventDesc> events);

    static void addEvent(Event event, std::function<void()> callback);
    static float getEventVal(Event event);
};
