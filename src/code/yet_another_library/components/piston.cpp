#include "piston.hpp"
#include "pros/adi.hpp"
#include <cstdint>
#include <sys/types.h>

Piston::Piston(Brain::Port port, std::initializer_list<EventDesc> events)
    : EventPort(
          events,
          [port](bool extended) { staticExtend(port, extended); },
          [port]() { return staticGetExtended(port); }),
      pistons({pros::adi::Pneumatics(static_cast<char>(port), false)}) {
    for (const auto &[event, extended] : events)
        Controller::addEvent(event, [*this, extended]() { extend(extended()); });
};

void Piston::extend(const bool extended) const {
    for (const auto &piston : pistons)
        piston.set_value(extended);
}
bool Piston::getExtended() const {
    int8_t average;
    for (const auto &piston : pistons)
        average += piston.is_extended() ? 1 : -1;

    return average > 0;
}
