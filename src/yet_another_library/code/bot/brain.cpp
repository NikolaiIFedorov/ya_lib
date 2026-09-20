#include "brain.hpp"
#include "pros/colors.hpp"
#include "pros/screen.h"
#include "pros/screen.hpp"

void Brain::log(size_t line, std::string msg, pros::Color color) {
    pros::screen::set_pen(color);
    pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, line, msg.c_str());
};
