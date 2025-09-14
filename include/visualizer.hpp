#pragma once
#include "triangles.hpp"
#include <map>
#include <vector>

using PointTy = double;

void run_visualizer(std::vector<triangle::Triangle<PointTy>> &input,
                    std::map<size_t, size_t> &intersections);
