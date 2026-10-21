#pragma once

#include "_ya_lib.hpp"

class Bot {
    using Config = ya_lib::Config &;

  public:
    static void init(Config config);
};
