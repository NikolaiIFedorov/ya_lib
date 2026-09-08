#pragma once

#include <functional>

class Log {
  public:
    static std::function<void()> getAuton(std::vector<std::function<void()>> autons);
};

#define LOG(...)