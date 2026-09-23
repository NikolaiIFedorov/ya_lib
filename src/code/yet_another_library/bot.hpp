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
        PortInstance(Brain::Port port, bool flipped = false) : port(port) {
            if (static_cast<int>(port) <= 20) {
                motorFromPort.insert({port, pros::Motor(static_cast<int>(port))});
                write = spin;
            } else {
                pneumaticsFromPort.insert(
                    {port, pros::adi::Pneumatics(static_cast<char>(port), false)});
                write = extend;
            }
        };

        void operator=(float pct) const {
            write(port, pct);
        };

        void addEvent(Controller::Event event, std::function<float(void)> function) {
            Controller::addEvent(event, [copy = *this, function]() { copy = function(); });
        };

      private:
        Brain::Port port;
        inline static std::map<Brain::Port, pros::Motor> motorFromPort;
        inline static std::map<Brain::Port, pros::adi::Pneumatics> pneumaticsFromPort;

        std::function<void(Brain::Port, float)> write;
        static void spin(Brain::Port port, float pct) {
            motorFromPort.at(port).move_voltage(pct * 12000);
        };
        static void extend(Brain::Port port, bool extended) {
            pneumaticsFromPort.at(port).set_value(extended);
        };
    };

    template <typename... MacroArgs> struct System {
        friend class PortInstance;

      public:
        explicit System(std::function<void(System &init)>) {
            init(*this);
        };

        void operator()(MacroArgs... macroArgs) {
            for (const auto &subMacro : subMacros)
                subMacro(macroArgs...);
        };

        void addSubMacro(PortInstance &portInstance, std::function<float(MacroArgs...)> subMacro) {
            subMacros.push_back([portInstance, subMacro](MacroArgs... macroArgs) {
                portInstance = subMacro(macroArgs...);
            });
        };

        PortInstance addPortInstance(Brain::Port port) {
            return PortInstance(port);
        };

      private:
        std::vector<std::function<void(MacroArgs...)>> subMacros;
    };

    static void init();
};
