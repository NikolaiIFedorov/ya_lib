#include "event.hpp"
#include "controller.hpp"

float Event::operator()() const {
    return Controller::getEventVal(id);
}
void Event::operator()(std::function<void()> callback) const {
    Controller::addEvent(id, callback);
}

constexpr Event Event::Left{Event::Id::Left};
constexpr Event Event::Right{Event::Id::Right};
constexpr Event Event::Up{Event::Id::Up};
constexpr Event Event::Down{Event::Id::Down};
constexpr Event Event::X{Event::Id::X};
constexpr Event Event::Y{Event::Id::Y};
constexpr Event Event::A{Event::Id::A};
constexpr Event Event::B{Event::Id::B};
constexpr Event Event::Ru{Event::Id::Ru};
constexpr Event Event::Rd{Event::Id::Rd};
constexpr Event Event::Lu{Event::Id::Lu};
constexpr Event Event::Ld{Event::Id::Ld};
constexpr Event Event::Rx{Event::Id::Rx};
constexpr Event Event::Ry{Event::Id::Ry};
constexpr Event Event::Lx{Event::Id::Lx};
constexpr Event Event::Ly{Event::Id::Ly};
constexpr Event Event::CompInit{Event::Id::CompInit};
constexpr Event Event::Disabled{Event::Id::Disabled};
constexpr Event Event::Auton{Event::Id::Auton};
constexpr Event Event::DriverControl{Event::Id::DriverControl};
