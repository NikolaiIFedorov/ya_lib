#pragma once

#include <iostream>
#include <source_location>
#include <type_traits>

class Log {

  public:
    template <typename Function, typename Return = std::invoke_result_t<Function>>
        requires std::is_invocable_r_v<Return, Function>
    static Return
    Trace(Function function, std::source_location location = std::source_location::current()) {

        std::string functionName = location.function_name();
        functionName = functionName.substr(functionName.find('/') + 1);
        std::string fileName = location.file_name();
        fileName = fileName.substr(fileName.find(' ') + 1);

        std::cout << fileName << ": " << functionName << " called\n";
        if constexpr (std::is_void_v<Return>) {
            function();
            std::cout << fileName << ": " << functionName << " returned\n";
            return;
        } else {
            const auto functionReturn = function();
            std::cout << fileName << ": " << functionName << " returned\n";
            return functionReturn;
        }
    };
};

inline float getSign(float num) {
    return num < 0 ? -1 : num > 0 ? 1 : 0;
};
