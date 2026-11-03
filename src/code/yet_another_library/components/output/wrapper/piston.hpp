#pragma once

#include "brain.hpp"
#include "pros/adi.hpp"

class ProsPiston : private pros::adi::Pneumatics {
  public:
    ProsPiston(const Brain::Port &port);

    void extend(const bool extended) const;
    bool getExtended() const;
};
