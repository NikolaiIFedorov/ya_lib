#include "piston.hpp"

ProsPiston::ProsPiston(const Port &port)
    : pros::adi::Pneumatics(static_cast<char>(port.port), false), flipped(port.flipped) {};

void ProsPiston::extend(const bool extended) const {
    set_value(flipped ? !extended : extended);
}
bool ProsPiston::getExtended() const {
    return flipped ? !is_extended() : is_extended();
}
