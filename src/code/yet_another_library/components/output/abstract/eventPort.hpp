#pragma once

#include "controller.hpp"
#include <functional>

class SystemPort {
  public:
    enum struct Kind { Motor, Piston };
    struct EventDesc {
        Controller::EventId event;
        std::function<float()> execute;
    };

    SystemPort(
        std::initializer_list<EventDesc> events,
        std::function<void(float)> write,
        std::function<float()> read);

    std::function<void(float)> write;
    std::function<float()> read;
};