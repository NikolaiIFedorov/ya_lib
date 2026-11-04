#pragma once

#include <functional>

class Event {
    friend class Controller;

  public:
    float operator()() const;
    void operator()(std::function<void()>) const;

    static const Event Left, Right, Up, Down, Lu, Ld, Ru, Rd, Y, X, A, B, Lx, Ly, Rx, Ry, CompInit,
        Disabled, Auton, DriverControl;

  private:
    enum struct Id {
        Left,
        Right,
        Up,
        Down,
        Lu,
        Ld,
        Ru,
        Rd,
        Y,
        X,
        A,
        B,
        Lx,
        Ly,
        Rx,
        Ry,
        CompInit,
        Disabled,
        Auton,
        DriverControl,
    };
    Id id;

    constexpr Event(Id eventId) : id(eventId) {};
};
