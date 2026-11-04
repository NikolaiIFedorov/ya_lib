#pragma once

#include "abstract/portDesc.hpp"
#include "pros/adi.hpp"

class ProsPiston : private pros::adi::Pneumatics {
  public:
    using Port = PortDesc::Letter;
    ProsPiston(const Port &port);

    void extend(const bool extended) const;
    bool getExtended() const;
};
