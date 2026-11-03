#include "math.hpp"

float Math::getSign(float num) {
    return num < 0 ? -1 : num > 0 ? 1 : 0;
};