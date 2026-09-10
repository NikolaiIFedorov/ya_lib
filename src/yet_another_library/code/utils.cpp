// Improve code quality
// Complete display log

#include "utils.hpp"
#include <iostream>

uint32_t Log::logCount = 0;

Log::Log(std::array<Section, 4> sections) {
    this->sections = sections;
};

Controller::Callback Log::getAuton(std::vector<Controller::Callback> autons) {
    return []() {};
};

std::string Log::getLabel(std::string str) {
    return DARK_GRAY + "[" + str + "]" + RESET;
};

std::string Log::getLabel(uint32_t num) {
    return getLabel(std::to_string(std::move(num)));
};

void Log::log(std::source_location loc, Kind kind, std::string msg) {
    auto *statusLogs = &sections[static_cast<int>(Log::Kind::Status)];
    statusLogs->addLog(msg);
    std::cout << msg << getLabel(logCount++)
              << getLabel(std::string(loc.file_name()) + ": " + std::string(loc.function_name()));
    displayLog(*statusLogs);
};

void Log::logFunctionStatus(std::source_location loc, bool returned) {
    log(loc,
        Kind::Trace,
        returned ? (std::string(loc.function_name()) + " returned")
                 : (std::string(loc.function_name()) + " called"));
};

Log::Section::Section(Kind kind, bool monoLine)
    : kind(kind), monoLine(monoLine), color(getColor(kind)), logLevel(getLogLevel(kind)) {};

const std::vector<std::string> &Log::Section::getLogs() const {
    return logs;
};

const Log::Kind &Log::Section::getKind() const {
    return kind;
};

const bool &Log::Section::getMonoLine() const {
    return monoLine;
};

const pros::Color &Log::Section::getColor() const {
    return color;
};

const bool &Log::Section::getLogLevel() const {
    return logLevel;
};

void Log::Section::addLog(std::string log) {
    logs.push_back(std::move(log));
}

pros::Color Log::Section::getColor(Log::Kind kind) {
    switch (kind) {
    case (Kind::Error):
        return pros::Color::red;
    case (Kind::Warn):
        return pros::Color::yellow;
    case (Kind::Trace):
        return pros::Color::blue;
    case (Kind::Status):
        return pros::Color::light_gray;
    };
};

bool Log::Section::getLogLevel(Log::Kind kind) {
    switch (kind) {
    case (Kind::Error):
        return true;
    case (Kind::Warn):
        return false;
    case (Kind::Trace):
        return true;
    case (Kind::Status):
        return false;
    };
};

void Log::displayLog(const Section &section) {

};