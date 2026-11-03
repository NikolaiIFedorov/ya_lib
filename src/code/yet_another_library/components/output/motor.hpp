#pragma once

#include "abstract/eventPort.hpp"
#include "brain.hpp"
#include "compensation/compensation.hpp"
#include "rotational.hpp"
#include "wrapper/motor.hpp"
#include <initializer_list>
#include <optional>

class Motor : public SystemPort {
  public:
    using Config = Compensation::Config;
    Motor(std::vector<Brain::Port> ports, std::initializer_list<EventDesc> events, Config config);

    void spin(float pct) const;
    void spin(float pct, float pos) const;
    float getVelocity() const;

    Config &getConfig() const;
    void setConfig(Config config);
    void setRotational(Brain::Port port);

  private:
    std::vector<ProsMotor> prosMotors;
    inline static std::vector<Motor *> motors;

    Compensation compensation;
    std::optional<Rotational> rotational;

    static void setVoltage(const Motor &motor, float pct);
    static void
    staticSpin(const Motor *motor, const float pct, const std::optional<float> pos = std::nullopt);
};