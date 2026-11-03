#pragma once

#include "abstract/subSystem.hpp"
#include "motor.hpp"
#include "piston.hpp"
#include "ya_lib.hpp"
#include <deque>

class Bot {
    using Config = ya_lib::Config &;
    using Port = Brain::Port;
    using Event = Controller::EventId;

  public:
    static void init(Config config);

  private:
    template <typename... MacroArgs>
    static SubSystem<MacroArgs...> addSystem(SubSystem<MacroArgs...>::Init init) {
        static std::deque<SubSystem<MacroArgs...>> subSystems;
        subSystems.emplace_back(init);
        return &subSystems.back();
    };

    static Motor addMotor(
        std::vector<Brain::Port> ports,
        std::initializer_list<Port::EventDesc> events,
        Motor::Config config);

    static Piston
    addPiston(std::vector<Brain::Port> ports, std::initializer_list<Port::EventDesc> events);

    static void addEvent(Event event, std::function<void()> callback);
    static float getEventVal(Event event);
};
