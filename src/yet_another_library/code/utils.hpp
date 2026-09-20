#pragma once

#include "bot/controller.hpp"
#include "pros/colors.hpp"
#include <array>
#include <cstdint>
#include <source_location>
#include <string>

enum class LogKind { ERROR, WARN, TRACE, STATUS, _COUNT };

constexpr size_t SECTION_COUNT = static_cast<size_t>(LogKind::_COUNT);

struct Sections;
struct Section;

class Log { // TODO: Make configurable
  public:
    Log(std::array<Section, SECTION_COUNT> sections);

    static Controller::Callback
    getAuton(std::vector<Controller::Callback> autons); // TODO: Complete getAuton
    static void createAuton();

    template <typename Function, typename Return = std::invoke_result_t<Function>>
    Return static _Trace(std::source_location loc, Function function) { // TODO: Add brain logging
        logFunctionStatus(loc, false);

        if constexpr (std::is_void_v<Return>) {
            function();
            logFunctionStatus(loc, true);
            return;
        } else {
            const Return returnVal = function();
            logFunctionStatus(loc, true);
            return returnVal;
        }
    }

  private:
    static Sections sections;

    static inline const std::string DARK_GRAY = "\033[90m";
    static inline const std::string ACCENT = "\033[90m";
    static inline const std::string RESET = "\033[0m";

    static uint32_t logCount;
    static uint16_t highestMsgLength;

    static std::string trimFunctionName(std::source_location loc);
    static std::string trimFileName(std::source_location loc);

    static std::string getLabel(std::string str);
    static std::string getLabel(uint32_t num);
    static std::string getAccent(std::string str);

    static void logFunctionStatus(std::source_location loc, bool returned);
    static void log(std::source_location loc, LogKind kind, std::string msg);
    static void displayLog(LogKind kind, std::string msg);
};

struct Section {
  public:
    Section() = default;
    Section(LogKind kind, bool monoLine = false);

    const std::vector<std::string> &getLogs() const;
    const LogKind &getKind() const;
    const bool &getMonoLine() const;
    const pros::Color &getColor() const;
    const bool &getLogLevel() const;

  private:
    LogKind kind;
    bool monoLine;
    pros::Color color;
    bool logLevel;

    pros::Color getColor(LogKind kind);
    bool getLogLevel(LogKind kind);
};

struct Sections : private std::array<Section, SECTION_COUNT> {
    Sections() = default;
    Sections(std::array<Section, SECTION_COUNT> sections);

    uint8_t addLog(LogKind kind);
    const Section &getSection(LogKind kind);

  private:
    uint8_t expandedSectionLineCount;
    std::map<LogKind, std::pair<size_t, size_t>> brainLineFromKind;
};

#define TRACE(...) Log::_Trace(std::source_location::current(), __VA_ARGS__)