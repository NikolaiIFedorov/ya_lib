#pragma once

#include "abstract/portDesc.hpp"
#include "wrapper/piston.hpp"
#include <initializer_list>
#include <vector>

class Piston : public PortDesc {
  public:
    using Port = PortDesc::Letter;
    Piston(std::vector<Port> port, std::initializer_list<EventDesc> events);

    void extend(const bool extended) const;
    bool getExtended() const;

  private:
    std::vector<ProsPiston> pistons;

    constexpr std::vector<uint8_t> getAdiPorts(const std::vector<Port> &ports);
};