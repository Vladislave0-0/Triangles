#pragma once

#include <cmath>

namespace triangle {
const double epsilon_ = 1e-6;

bool cmp(double x, double y);

size_t calculate_octotree_depth(size_t triag_num);
} // namespace triangle
