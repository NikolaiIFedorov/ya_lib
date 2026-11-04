#include "portDesc.hpp"

PortDesc::PortDesc(
    std::initializer_list<EventDesc> events,
    std::function<void(float)> write,
    std::function<float()> read)
    : write(write), read(read) {
    for (const auto &[event, arg] : events)
        event([write = this->write, read = this->read, arg]() { write(arg(read())); });
};

PortDesc::EventDesc::EventDesc(const Event &event, std::function<float()> callback)
    : event(event), callback([callback](float state) { return callback(); }) {};
PortDesc::EventDesc::EventDesc(const Event &event, std::function<float(float)> callback)
    : event(event), callback(callback) {};

float PortDesc::operator()() const {
    return read();
}
void PortDesc::operator()(float arg) const {
    write(arg);
}