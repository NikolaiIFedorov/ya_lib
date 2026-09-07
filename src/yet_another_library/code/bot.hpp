#pragma once

#include "bot/controller.hpp"
#include "bot/output.hpp"
#include <array>
#include <functional>
#include <map>
#include <vector>

class Bot {
  public:
    using Equation = std::function<float()>;
    using Outputs = std::vector<Output>;
    template <size_t subsystemCount>
    using Macro = std::function<float(std::array<float, subsystemCount>)>;

    template <size_t subsystemCount>
    using Subsystems = std::
        map<Outputs, std::pair<std::map<Controller::InputCmd, Equation>, Macro<subsystemCount>>>;

    template <size_t subsystemCount> class System : public Subsystems<subsystemCount> {
      public:
        using Macro = Bot::Macro<subsystemCount>;
        using Subsystems = Bot::Subsystems<subsystemCount>;

        System(Subsystems subsystems);
        void operator()(std::array<float, subsystemCount> macroArgs) const;
    };

    static void init();
    static void drivercontrol();

    template <size_t subsystemCount>
    static const System<subsystemCount> addSystem(System<subsystemCount> system);
};
