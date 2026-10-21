#pragma once

#include "brain.hpp"
#include "eventPort.hpp"
#include "pros/adi.hpp"
#include <initializer_list>
#include <vector>

class ProsPiston : private pros::adi::Pneumatics {
  public:
    ProsPiston(const Brain::Port &port);

    void extend(const bool extended) const;
    bool getExtended() const;
};

class Piston : private EventPort {
  public:
    Piston(std::vector<Brain::Port> port, std::initializer_list<EventDesc> events);

    void extend(const bool extended) const;
    bool getExtended() const;

  private:
    std::vector<ProsPiston> pistons;

    constexpr std::vector<uint8_t> getAdiPorts(const std::vector<Brain::Port> &ports);
};