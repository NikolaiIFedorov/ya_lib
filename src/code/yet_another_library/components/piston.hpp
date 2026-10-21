#pragma once

#include "brain.hpp"
#include "eventPort.hpp"
#include "pros/adi.hpp"
#include <initializer_list>
#include <vector>

class Piston : private EventPort {
  public:
    Piston(Brain::Port port, std::initializer_list<EventDesc> events);
    Piston(std::vector<Brain::Port> port, std::initializer_list<EventDesc> events);

    void extend(const bool extended) const;
    bool getExtended() const;

  private:
    std::vector<pros::adi::Pneumatics> pistons;

    static void staticExtend(const Brain::Port port, const bool extended);
    static float staticGetExtended(const Brain::Port port);
};