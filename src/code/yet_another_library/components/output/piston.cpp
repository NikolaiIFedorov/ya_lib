#include "piston.hpp"
#include <cstdint>
#include <sys/types.h>

Piston::Piston(std::vector<Port> ports, std::initializer_list<EventDesc> events)
    : PortDesc(
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
        average += (piston.getExtended() ? 1 : -1);

    return average > 0;
}

constexpr std::vector<uint8_t> Piston::getAdiPorts(const std::vector<Port> &ports) {
    std::vector<char> charPorts;
    for (const auto &port : ports)
        charPorts.push_back(static_cast<char>(port.port));

    return std::vector<uint8_t>(charPorts.begin(), charPorts.end());
}
