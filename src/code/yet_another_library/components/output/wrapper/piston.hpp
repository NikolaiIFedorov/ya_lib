#pragma once

#include "abstract/port.hpp"
#include "pros/adi.hpp"

class ProsPiston : private pros::adi::Pneumatics {
  public:
    using Port = ::PortInstance::Letter;
    ProsPiston(const ::PortInstance &port);

    void extend(const bool extended) const;
    bool getExtended() const;
};
