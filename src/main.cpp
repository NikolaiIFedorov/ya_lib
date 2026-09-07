#include "main.h"
#include "yet_another_library/code/bot.hpp"

void initialize() {
    Bot::init();
}

void disabled() {}

void competition_initialize() {
    Bot::_compInit();
}

void autonomous() {
    Bot::_auton();
}

void opcontrol() {
    Bot::_drivercontrol();
}