#include "main.h"
#include "code/yet_another_library/_ya_lib.hpp"

void initialize() {
    _ya_lib::init();
};

void disabled() {
    _ya_lib::disabled();
}

void competition_initialize() {
    _ya_lib::compInit();
}

void autonomous() {
    _ya_lib::auton();
}

void opcontrol() {
    _ya_lib::drivercontrol();
}