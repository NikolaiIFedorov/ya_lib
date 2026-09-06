#pragma once

#include "bot/controller.hpp"
#include <functional>
#include <map>

class Bot {
  public:
    using Port = Brain::Port;

    struct Port_InputAmp_Map {
        using Map = std::map<Port, float>;

        ReactOutput(Map map);
        ReactOutput(std::vector<Port> ports);
        ReactOutput(Port port);

        Map map;
    };

    using System = std::map<Controller::InputCmd, Port_InputAmp_Map>;

    static void init();

    static void addSystem(System system);
};