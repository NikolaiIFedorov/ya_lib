#pragma once

#include "event.hpp"
#include <functional>

class PortDesc {
    friend class Motor;
    friend class Piston;
    friend class Inertial;
    friend class Rotational;

  public:
    enum struct Number {
        _1 = 1,
        _2 = 2,
        _3 = 3,
        _4 = 4,
        _5 = 5,
        _6 = 6,
        _7 = 7,
        _8 = 8,
        _9 = 9,
        _10 = 10,
        _11 = 11,
        _12 = 12,
        _13 = 13,
        _14 = 14,
        _15 = 15,
        _16 = 16,
        _17 = 17,
        _18 = 18,
        _19 = 19,
        _20 = 20
    };
    enum struct Letter {
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
    };
    struct EventDesc {
        EventDesc(const Event &event, std::function<float()> callback);
        EventDesc(const Event &event, std::function<float(float)> callback);

        const Event &event;
        std::function<float(float state)> callback;
    };

    float operator()() const;
    void operator()(float arg) const;

  private:
    PortDesc(
        std::initializer_list<EventDesc> events,
        std::function<void(float)> write,
        std::function<float()> read);

    std::function<void(float)> write;
    std::function<float()> read;
};

template <class Port> struct Flippable {
  public:
    constexpr Flippable(Port port, bool flipped = false) : port(port), flipped(flipped) {};
    Port port;
    bool flipped;
};

constexpr Flippable<PortDesc::Number> operator-(PortDesc::Number number) {
    return {number, true};
}

constexpr Flippable<PortDesc::Letter> operator-(PortDesc::Letter letter) {
    return {letter, true};
}
