#include "brain.hpp"

void Brain::log(size_t line, std::string msg) {
    lines[line - 1] = std::move(msg);
};

size_t Brain::getLineCapacity() {
    return lines.size();
}