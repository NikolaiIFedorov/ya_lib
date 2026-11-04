#include "piston.hpp"

ProsPiston::ProsPiston(const Port &port) : pros::adi::Pneumatics(static_cast<char>(port), false) {};

void ProsPiston::extend(const bool extended) const {
    set_value(extended);
}
bool ProsPiston::getExtended() const {
    return is_extended();
}
