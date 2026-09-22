#pragma once
#include "components/brain.hpp"
#include "components/controller.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

#include <functional>
#include <map>

class Bot {
  public:
    class PortInstance {

      public:
        PortInstance(Brain::Port port, bool flipped = false) {
            if (static_cast<int>(port) <= 20) {
                motorFromPort.insert({port, pros::Motor(static_cast<int>(port))});
                *this = PortInstance(spin);
            } else {
                pneumaticsFromPort.insert(
                    {port, pros::adi::Pneumatics(static_cast<char>(port), false)});
                *this = PortInstance(extend);
            }
        };

        void operator=(float pct) const {
            write(port, pct);
        };

        void addEvent(Controller::Event event, std::function<float(void)> function) {
            Controller::addEvent(event, [this, function]() { (*this) = function(); });
        };

      private:
        Brain::Port port;
        static std::map<Brain::Port, pros::Motor> motorFromPort;
        static std::map<Brain::Port, pros::adi::Pneumatics> pneumaticsFromPort;

        PortInstance(std::function<void(Brain::Port, float)> write) : write(write) {};
        std::function<void(Brain::Port, float)> write;
        static void spin(Brain::Port port, float pct) {
            motorFromPort[port].move_voltage(pct * 12000);
        };
        static void extend(Brain::Port port, bool extended) {
            pneumaticsFromPort[port].set_value(extended);
        };
    };

    template <typename... MacroArgs> struct System {
        friend class PortInstance;

      public:
        System(std::function<void()> init) {
            constructingSystem = this;
            init();
            constructingSystem = nullptr;
        }

        void operator()(MacroArgs... macroArgs) {
            for (const auto &subMacro : subMacros)
                subMacro(macroArgs...);
        };

        static void
        addSubMacro(PortInstance &portInstance, std::function<float(MacroArgs...)> subMacro) {
            if (constructingSystem == nullptr)
                return;

            constructingSystem->subMacros.push_back(
                [portInstance, subMacro](MacroArgs... macroArgs) {
                    portInstance = subMacro(macroArgs...);
                });
        };

      private:
        inline static System *constructingSystem = nullptr;
        std::vector<std::function<void(MacroArgs...)>> subMacros;
    };

    static void init();
};

inline Bot::System<float> _system([]() {
    Bot::PortInstance instance(Brain::Port::_1);
    instance.addEvent(Controller::Event::Rd, []() { return 1; });

    _system.addSubMacro(instance, [](float input) { return input; });
});
