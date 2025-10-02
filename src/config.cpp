#include "config.hpp"

namespace triangle {
bool cmp(double x, double y) { return fabs(x - y) <= epsilon_; }

size_t calculate_octotree_depth(size_t triag_num) {
    if (triag_num < 1000) {
        return 0;
    }
    return static_cast<size_t>(std::log10(triag_num)) - 2;
}

} // namespace triangle
