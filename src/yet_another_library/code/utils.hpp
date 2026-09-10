#pragma once

#include "bot/controller.hpp"
#include "pros/colors.hpp"
#include <cstdint>
#include <source_location>
#include <string>

class Log { // Make configurable
  public:
    enum class Kind {
        Error,
        Warn,
        Trace,
        Status,
    };

    struct Section {
      public:
        Section(Kind kind, bool monoLine = false);

        void addLog(std::string log);

        const std::vector<std::string> &getLogs() const;
        const Kind &getKind() const;
        const bool &getMonoLine() const;
        const pros::Color &getColor() const;
        const bool &getLogLevel() const;

      private:
        std::vector<std::string> logs;

        Kind kind;
        bool monoLine;
        pros::Color color;
        bool logLevel;

        pros::Color getColor(Kind kind);
        bool getLogLevel(Kind kind);
    };

    Log(std::array<Section, 4> sections);

    static Controller::Callback
    getAuton(std::vector<Controller::Callback> autons); // Complete getAuton

    template <typename Function, typename Re = std::invoke_result_t<Function>>
    Re static _Trace(std::source_location loc, Function function) { // Add brain logging
        logFunctionStatus(loc, false);

        if constexpr (std::is_void_v<Re>) {
            function();
            logFunctionStatus(loc, true);
            return;
        } else {
            const Re returnVal = function();
            logFunctionStatus(loc, true);
            return returnVal;
        }
    }

  private:
    static inline const std::string DARK_GRAY = "\033[90m";
    static inline const std::string RESET = "\033[0m";

    static uint32_t logCount;

    static void log(std::source_location loc, Kind kind, std::string msg);

    static std::string getLabel(std::string str);
    static std::string getLabel(uint32_t num);
    static void logFunctionStatus(std::source_location loc, bool returned);

    static std::array<Section, 4> sections;
    static void displayLog(const Section &section);
};

#define TRACE(...) Log::_Trace(std::source_location::current(), __VA_ARGS__)