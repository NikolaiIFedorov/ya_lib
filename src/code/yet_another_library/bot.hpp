#pragma once

#include "_ya_lib.hpp"
#include <deque>

class Bot {
  public:
    template <typename... MacroArgs> using ConstSystem = const _ya_lib::System<MacroArgs...> *;
    template <typename... MacroArgs>
    static ConstSystem<MacroArgs...> addSystem(_ya_lib::System<MacroArgs...>::Init init) {
        static std::deque<_ya_lib::System<MacroArgs...>> systems;
        systems.push_back(_ya_lib::System<MacroArgs...>(init));
        return &systems.back();
    };

    using ConstPortInstance = const _ya_lib::PortInstance *;
    static ConstPortInstance addPortInstance(std::vector<Brain::Port> ports) {
        static std::deque<_ya_lib::PortInstance> portInstances;
        portInstances.push_back(_ya_lib::PortInstance{ports});
        return &portInstances.back();
    };

    static void
    addEvent(ConstPortInstance portInstance, Controller::Event event, std::function<float()> pct) {
        Controller::addEvent(event, [portInstance, pct]() { (*portInstance) = pct(); });
    };

    static void init();
};
