#include "port.hpp"

PortInstance::PortInstance(
    std::initializer_list<EventDesc> events,
    std::function<void(float)> write,
    std::function<float()> read)
    : write(write), read(read) {
    for (const auto &[event, arg] : events)
        event([this, arg]() { (*this)(arg()); });
};

float PortInstance::operator()() {
    return read();
}
void PortInstance::operator()(float arg) {
    write(arg);
}