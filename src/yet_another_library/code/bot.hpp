#pragma once

#include "bot/controller.hpp"
#include "bot/output.hpp"
#include <functional>
#include <map>
#include <vector>

class Bot {
  public:
    using Equation = std::function<float()>;
    using Outputs = std::vector<Output>;
    template <typename... Args> using Macro = std::function<float(Args...)>;

    template <typename... MacroArgs>
    using Subsystems =
        std::map<Outputs, std::pair<std::map<Controller::InputCmd, Equation>, Macro<MacroArgs...>>>;

    template <typename... MacroArgs> class System : public Subsystems<MacroArgs...> {
      public:
        using Macro = Bot::Macro<MacroArgs...>;
        using Subsystems = Bot::Subsystems<MacroArgs...>;

        System(Subsystems subsystems);
        void operator()(MacroArgs...) const;
    };

    static void init();
    static void drivercontrol();

    template <typename... MacroArgs>
    static System<MacroArgs...> addSystem(System<MacroArgs...> system);
};
