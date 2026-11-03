#pragma once

#include "abstract/port.hpp"
#include "wrapper/piston.hpp"
#include <initializer_list>
#include <vector>

class Piston : public PortInstance {
  public:
    enum struct Port {
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
    };
    Piston(std::vector<Port> port, std::initializer_list<EventDesc> events);

    void extend(const bool extended) const;
    bool getExtended() const;

  private:
    std::vector<ProsPiston> pistons;

    constexpr std::vector<uint8_t> getAdiPorts(const std::vector<Port> &ports);
};