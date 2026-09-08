#include "utils.hpp"
#include <iostream>

std::string Log::getLabel(std::string str) {
    return "[" + str + "]";
};

std::string Log::getLabel(uint32_t num) {
    return getLabel(std::to_string(num));
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