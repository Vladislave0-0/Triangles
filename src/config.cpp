#include "../include/config.hpp"

bool float_cmp(float x, float y) { return fabs(x - y) < epsilon; }