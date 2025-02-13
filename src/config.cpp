#include "../include/config.hpp"
namespace triangle {
bool cmp(double x, double y) { return fabs(x - y) <= epsilon_; }
} // namespace triangle
