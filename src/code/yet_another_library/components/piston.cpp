#include "piston.hpp"
#include "pros/adi.hpp"
#include <cstdint>
#include <sys/types.h>

ProsPiston::ProsPiston(const Brain::Port &port)
    : pros::adi::Pneumatics(static_cast<char>(port), false) {};

void ProsPiston::extend(const bool extended) const {
    set_value(extended);
}
bool ProsPiston::getExtended() const {
    return is_extended();
}

Piston::Piston(std::vector<Brain::Port> ports, std::initializer_list<EventDesc> events)
    : EventPort(
          events,
          [this](bool extended) { extend(extended); },
          [this]() { return getExtended(); }),
      pistons(std::vector<ProsPiston>(ports.begin(), ports.end())) {};

void Piston::extend(const bool extended) const {
    for (const auto &piston : pistons)
        piston.extend(extended);
}
bool Piston::getExtended() const {
    int8_t average;
    for (const auto &piston : pistons)
        average += piston.getExtended() ? 1 : -1;

    return average > 0;
}

constexpr std::vector<uint8_t> Piston::getAdiPorts(const std::vector<Brain::Port> &ports) {
    std::vector<char> charPorts;
    for (const auto &port : ports)
        charPorts.push_back(static_cast<char>(port));

    return std::vector<uint8_t>(charPorts.begin(), charPorts.end());
}
