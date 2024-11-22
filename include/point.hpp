#pragma once

#include "config.hpp"
#include <iostream>
#include <math.h>

template <typename PointTy = double> class Point {
  PointTy x, y, z = NAN;

public:
  bool valid() const {
    return !std::isnan(x) && !std::isnan(y) && !std::isnan(z);
  }

  PointTy get_x() const { return x; }
  PointTy get_y() const { return y; }
  PointTy get_z() const { return z; }

  Point() = default;

  Point(PointTy x, PointTy y, PointTy z) : x(x), y(y), z(z){};

  void print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
  }

  void print_no_endl() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")";
  }

  PointTy norm() const { return x * x + y * y + z * z; }

  bool operator==(const Point<PointTy> &other) const {
    return double_cmp(x, other.x) && double_cmp(y, other.y) &&
           double_cmp(z, other.z);
  }

  bool operator!=(const Point<PointTy> &other) const {
    return !double_cmp(x, other.x) || !double_cmp(y, other.y) ||
           !double_cmp(z, other.z);
  }

  Point<PointTy> operator+(const Point<PointTy> &other) const {
    Point<PointTy> add(x + other.x, y + other.y, z + other.z);
    return add;
  }

  Point<PointTy> operator-(const Point<PointTy> &other) const {
    Point<PointTy> sub(x - other.x, y - other.y, z - other.z);
    return sub;
  }

  void operator=(const Point<PointTy> &other) {
    x = other.x;
    y = other.y;
    z = other.z;
  }
};
