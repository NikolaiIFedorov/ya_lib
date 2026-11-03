#pragma once

#include "abstract/eventPort.hpp"
#include "brain.hpp"
#include "wrapper/piston.hpp"
#include <initializer_list>
#include <vector>

class Piston : public SystemPort {
  public:
    Piston(std::vector<Brain::Port> port, std::initializer_list<EventDesc> events);

    void extend(const bool extended) const;
    bool getExtended() const;

  private:
    std::vector<ProsPiston> pistons;

    constexpr std::vector<uint8_t> getAdiPorts(const std::vector<Brain::Port> &ports);
};