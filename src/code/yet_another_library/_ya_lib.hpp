#pragma once

#include "components/brain.hpp"
#include "components/controller.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "utils/utils.hpp"
#include <functional>

// -TODO: Add pid
// -TODO: Add BaseOp util
// -TODO: Add PortInstance groups
// ?TODO: Add port pattern
// TODO: Add pid
// -TODO: Add logging
// TODO: Add device logging
// TODO: Support Auton selection
// TODO: Add background stuff
// TODO: Improve code quality
// TODO: Improve syntax
// -TODO: Force System const
// TODO: Support Device
// TODO: Add auton compiler

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

        static uint16_t getRpm(const pros::Motor &motor) {
            switch (motor.get_gearing()) {
            case pros::MotorGears::rpm_100:
                return 100;
            case pros::MotorGears::rpm_200:
                return 200;
            case pros::MotorGears::rpm_600:
                return 600;
            default:
                return 200;
            };
        };
        static float getPct(const pros::Motor &motor) {
            return motor.get_actual_velocity() / getRpm(motor);
        }
        static float getPct(const Brain::Port port) {
            return getPct(motorPorts.at(port));
        }

        static void movePct(const pros::Motor &motor, float pct) {
            motor.move_voltage(pct * 12000);
        };

        inline static float kp = 1;
        inline static float ki = 1;
        static float pid(const pros::Motor &motor, const float targetPct) {
            static pros::MutexVar<std::map<uint8_t, float>> integralPorts;

            const auto diff = targetPct - getPct(motor);
            const auto integral = (*integralPorts.lock()).at(motor.get_port()) += diff * dt;

            return kp * diff + ki * integral;
        };

        inline static float ks = 1;
        inline static float kv = 1;
        inline static float ka = 0;
        static float ff(const pros::Motor &motor, const float targetPct, const float acceleration) {
            const float pctSign = getSign(targetPct);

            return ks * pctSign + kv * targetPct + ka * acceleration;
        };

        inline static float ca = 1;
        // TODO: Take in account for lag
        static bool accelerate(const Brain::Port port, const float targetPct) {
            const auto &motor = motorPorts.at(port);

            const auto currentPct = getPct(motor);
            const auto pctDiff = targetPct - currentPct;
            if (std::abs(pctDiff) > ca) {
                const auto signCa = getSign(pctDiff) * ca;
                const auto pctStep = currentPct + signCa;
                movePct(motor, pid(motor, pctStep) + ff(motor, pctStep, signCa / dt));
                return false;
            } else {
                movePct(motor, pid(motor, targetPct) + ff(motor, targetPct, pctDiff / dt));
                return true;
            }
        };

        inline static constexpr float tick = 10;
        inline static constexpr float dt = 10 * 0.001;
        static void spin(Brain::Port port, float pct) {
            Log::Trace([port, pct]() {
                static pros::MutexVar<std::map<Brain::Port, float>> targetPctPorts;

                (*targetPctPorts.lock()).at(port) = pct;

                static bool pidTaskRun = false;
                if (pidTaskRun)
                    return;

                pros::Task ffPid{[]() {
                    pidTaskRun = true;
                    while (pidTaskRun) {
                        const auto currentTargetPctPorts = *targetPctPorts.lock();
                        for (const auto &[port, targetPct] : currentTargetPctPorts) {
                            if (accelerate(port, targetPct))
                                (*targetPctPorts.lock()).erase(port);
                        };
                        pros::delay(tick);
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
                pros::delay(tick);
            };
        });
    };
};
