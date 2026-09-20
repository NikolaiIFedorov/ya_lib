// TODO: Improve code quality
// TODO: Complete display log

#include "utils.hpp"
#include "bot/brain.hpp"
#include <iostream>
#include <source_location>

Sections Log::sections;
uint32_t Log::logCount;
uint16_t Log::highestMsgLength;

Log::Log(std::array<Section, SECTION_COUNT> sections) {
    this->sections = Sections(sections);
};

Controller::Callback Log::getAuton(std::vector<Controller::Callback> autons) {
    return []() {};
};

std::string Log::getLabel(std::string str) {
    return DARK_GRAY + "[" + str + "]" + RESET;
};

std::string Log::getAccent(std::string str) {
    return ACCENT + str + RESET;
};

std::string Log::getLabel(uint32_t num) {
    return getLabel(std::to_string(std::move(num)));
};

std::string Log::trimFunctionName(std::source_location loc) {
    std::string functionName = std::string(loc.function_name());
    return functionName.substr(functionName.find_last_of(' ') + 1);
};

std::string Log::trimFileName(std::source_location loc) {
    std::string fileName = std::string(loc.file_name());
    return fileName.substr(fileName.find_last_of('/') + 1);
};

void Log::log(std::source_location loc, LogKind kind, std::string msg) {
    std::string labels =
        getLabel(logCount++) + getLabel(trimFileName(loc) + ": " + trimFunctionName(loc));

    uint16_t msgLength = msg.length();
    if (msgLength > highestMsgLength)
        highestMsgLength = msgLength;

    std::string spacing(highestMsgLength - msgLength + 1, ' ');

    std::cout << msg << spacing << labels << "\n";
    displayLog(kind, msg);
};

void Log::logFunctionStatus(std::source_location loc, bool returned) {
    log(loc, LogKind::TRACE, returned ? "Function return" : "Function call");
};

Section::Section(LogKind kind, bool monoLine)
    : kind(kind), monoLine(monoLine), color(getColor(kind)), logLevel(getLogLevel(kind)) {};

const LogKind &Section::getKind() const {
    return kind;
};

const bool &Section::getMonoLine() const {
    return monoLine;
};

const pros::Color &Section::getColor() const {
    return color;
};

const bool &Section::getLogLevel() const {
    return logLevel;
};

pros::Color Section::getColor(LogKind kind) {
    switch (kind) {
    case (LogKind::ERROR):
        return pros::Color::red;
    case (LogKind::WARN):
        return pros::Color::yellow;
    case (LogKind::TRACE):
        return pros::Color::blue;
    case (LogKind::STATUS):
        return pros::Color::light_gray;
    default:
        return pros::Color::light_gray;
    };
};

bool Section::getLogLevel(LogKind kind) {
    switch (kind) {
    case (LogKind::ERROR):
        return true;
    case (LogKind::WARN):
        return false;
    case (LogKind::TRACE):
        return true;
    case (LogKind::STATUS):
        return false;
    default:
        return false;
    };
};

Sections::Sections(std::array<Section, SECTION_COUNT> sections) {
    uint8_t monoLineCount;
    uint8_t expandedLineCount;
    for (const Section &section : sections) {
        if (section.getMonoLine())
            monoLineCount++;
        else
            expandedLineCount++;

        (*this)[static_cast<uint8_t>(section.getKind())] = section;
    };

    expandedSectionLineCount = (Brain::screenCapacity - monoLineCount) / expandedLineCount;
};

const Section &Sections::getSection(LogKind kind) {
    return (*this)[static_cast<uint8_t>(kind)];
};

uint8_t Sections::addLog(LogKind kind) {
    if (getSection(kind).getMonoLine())
        return brainLineFromKind[kind].first;

    auto &lines = brainLineFromKind[kind].second;
    const auto start = brainLineFromKind[kind].first;
    if (lines == expandedSectionLineCount) {
        return start + (lines = 1);
    }

    return start + lines++;
};

void Log::displayLog(LogKind kind, std::string msg) {
    if (!sections.getSection(kind).getLogLevel())
        return;

    Brain::log(sections.addLog(kind), msg, sections.getSection(kind).getColor());
};