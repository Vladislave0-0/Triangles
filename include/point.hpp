#pragma once

#include "config.hpp"
#include <iostream>
#include <math.h>

template <typename PointTy = float> class Point {
  PointTy x, y, z = NAN;

public:
  bool operator==(const Point<PointTy> &other) const {
    return float_cmp(x, other.x) && float_cmp(y, other.y) &&
           float_cmp(z, other.z);
  }

  bool valid() const { return !isnan(x * y * z); }

  Point(PointTy x, PointTy y, PointTy z) : x(x), y(y), z(z){};

  void print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
  }

  PointTy norm() const { return x * x + y * y + z * z; }
};
