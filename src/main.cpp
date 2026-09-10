#include "main.h"
#include "yet_another_library/code/bot.hpp"

void initialize() {
    Log(
        {Log::Section{Log::Kind::Error},
         Log::Section{Log::Kind::Warn},
         Log::Section{Log::Kind::Trace, true},
         Log::Section{Log::Kind::Status, true}});
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