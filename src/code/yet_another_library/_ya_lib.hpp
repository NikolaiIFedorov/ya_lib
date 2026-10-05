#pragma once

#include "components/brain.hpp"
#include "components/controller.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "utils.hpp"
#include <functional>

// -TODO: Add pid
// -TODO: Add BaseOp util
// -TODO: Add PortInstance groups
// ?TODO: Add port pattern
// TODO: Complete PID
// -TODO: Add logging
// TODO: Add device logging
// TODO: Support Auton selection
// TODO: Add background stuff
// TODO: Improve code quality
// TODO: Improve syntax
// -TODO: Force System const
// TODO: Support Device

class _ya_lib {
  public:
    class PortInstance : public BaseOp {

      public:
        using BaseOp::operator=;

        BaseOp baseOpInit(Brain::Port port) {
            return Log::Trace([port]() {
                if (static_cast<int>(port) <= 20) {
                    motorPorts.insert({port, pros::Motor(static_cast<int>(port))});

                    return BaseOp(
                        [port]() { return getPct(port); },
                        [port](float pct) { spin(port, pct); });
                } else {
                    pneumaticPorts.insert(
                        {port, pros::adi::Pneumatics(static_cast<char>(port), false)});
                    return BaseOp(
                        [port]() { return getExtended(port); },
                        [port](float extended) { extend(port, extended); });
                }
            });
        };

        BaseOp baseOpInit(std::vector<Brain::Port> ports) {
            const auto portInstances = std::vector<PortInstance>{ports.begin(), ports.end()};
            return BaseOp(std::vector<BaseOp>{portInstances.begin(), portInstances.end()});
        };

        PortInstance(Brain::Port port, bool flipped = false)
            : ports({port}), BaseOp(baseOpInit(port)) {};
        PortInstance(std::vector<Brain::Port> ports) : ports(ports), BaseOp(baseOpInit(ports)) {};

        void addEvent(Controller::Event event, std::function<float(void)> function) {
            Controller::addEvent(event, [copy = *this, function]() { copy = function(); });
        };

      private:
        const std::vector<Brain::Port> ports;

        inline static std::map<Brain::Port, pros::Motor> motorPorts;
        static float getPct(Brain::Port port) {
            return motorPorts.at(port).get_voltage() / 12000.0f;
        }

        inline static pros::MutexVar<std::map<Brain::Port, float>> targetPctPorts;

        static void movePct(const pros::Motor &motor, float pct) {
            motor.move_voltage(pct * 12000.0f);
        };

        inline static const float pctStep = 0.1f;
        static void spin(Brain::Port port, float pct) {
            Log::Trace([port, pct]() {
                (*targetPctPorts.lock())[port] = pct;
                pros::Task pid{[]() {
                    while (true) {
                        const auto targetPctPortsCopy = *targetPctPorts.lock();
                        for (const auto &[port, targetPct] : targetPctPortsCopy) {
                            const auto currentPct = getPct(port);
                            const auto pctDiff = targetPct - currentPct;
                            if (std::abs(pctDiff) < pctStep) {
                                movePct(motorPorts.at(port), targetPct);
                            } else {
                                movePct(
                                    motorPorts.at(port),
                                    currentPct + (pctDiff > 0 ? pctStep : -pctStep));
                            }
                        }
                        pros::delay(10);
                    }
                }};
            });
        }

        inline static std::map<Brain::Port, pros::adi::Pneumatics> pneumaticPorts;
        static bool getExtended(Brain::Port port) {
            return pneumaticPorts.at(port).is_extended();
        };
        static void extend(Brain::Port port, bool extended) {
            pneumaticPorts.at(port).set_value(extended);
        };
    };

    template <typename... MacroArgs> struct System {
        friend class PortInstance;

      public:
        using Init = std::function<void(System &)>;
        explicit System(Init init) {
            init(*this);
        };

        void operator()(MacroArgs... macroArgs) const {
            const auto &subMacros = this->subMacros;
            Log::Trace([subMacros, macroArgs...] {
                for (const auto &subMacro : subMacros)
                    subMacro(macroArgs...);
            });
        };

        void addSubMacro(PortInstance &portInstance, std::function<float(MacroArgs...)> subMacro) {
            const auto &subMacros = this->subMacros;
            Log::Trace([subMacros, portInstance, subMacro] {
                subMacros.push_back([portInstance, subMacro](MacroArgs... macroArgs) {
                    portInstance = subMacro(macroArgs...);
                });
            });
        };

      private:
        std::vector<std::function<void(MacroArgs...)>> subMacros;
    };

    static void init();

    static void compInit() {
        Controller::executeEvents(Controller::Event::CompInit);
    };
    static void disabled() {
        Controller::executeEvents(Controller::Event::Disabled);
    };
    static void auton() {
        Controller::executeEvents(Controller::Event::Auton);
    };
    static void drivercontrol() {
        Log::Trace([]() {
            Controller::executeEvents(Controller::Event::DriverControl);
            while (true) {
                Controller::executeEvents();
                pros::delay(10);
            };
        });
    };
};
