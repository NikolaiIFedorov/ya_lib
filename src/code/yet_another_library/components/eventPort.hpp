#pragma once

#include "controller.hpp"
#include <functional>

class EventPort {
  public:
    struct EventDesc {
        Controller::Event event;
        std::function<float()> execute;
    };

    EventPort(
        std::initializer_list<EventDesc> events,
        std::function<void(float)> write,
        std::function<float()> read);

    std::function<void(float)> write;
    std::function<float()> read;
};