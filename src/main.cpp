#include "main.h"
#include "code/yet_another_library/ya_lib.hpp"

void initialize() {
    ya_lib::init();
};

void disabled() {
    ya_lib::disabled();
}

void competition_initialize() {
    ya_lib::compInit();
}

void autonomous() {
    ya_lib::auton();
}

void opcontrol() {
    ya_lib::drivercontrol();
}