#pragma once

#include <cstdint>
#include <functional>
#include <source_location>
#include <string>

class Log {
  public:
    static std::function<void()> getAuton(std::vector<std::function<void()>> autons);

    template <typename Function, typename Re = std::invoke_result_t<Function>>
    Re static _Trace(std::source_location loc, Function function) {
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
    static uint32_t logCount;

    static std::string getLabel(std::string str);
    static std::string getLabel(uint32_t num);
    static void log(std::source_location loc, std::string msg);
    static void logFunctionStatus(std::source_location loc, bool returned);
};

#define TRACE(...) Log::_Trace(std::source_location::current(), __VA_ARGS__)