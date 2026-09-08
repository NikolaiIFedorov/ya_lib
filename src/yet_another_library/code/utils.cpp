#include "utils.hpp"
#include <iostream>

uint32_t Log::logCount = 0;

Controller::Callback Log::getAuton(std::vector<Controller::Callback> autons) {
    return []() {};
};

std::string Log::getLabel(std::string str) {
    return DARK_GRAY + "[" + str + "]" + RESET;
};

std::string Log::getLabel(uint32_t num) {
    return getLabel(std::to_string(std::move(num)));
};

void Log::log(std::source_location loc, std::string msg) {
    std::cout << msg << getLabel(logCount++)
              << getLabel(std::string(loc.file_name()) + ": " + std::string(loc.function_name()));
};

void Log::logFunctionStatus(std::source_location loc, bool returned) {
    log(loc,
        returned ? (std::string(loc.function_name()) + " returned")
                 : (std::string(loc.function_name()) + " called"));
};