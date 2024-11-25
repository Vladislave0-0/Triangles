#pragma once

#include <iostream>
#include <math.h>

#include "config.hpp"

template <typename PointTy> class Triangle;
template <typename PointTy> class Vector;

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

template <typename PointTy = double>
bool three_points_on_one_line(const Point<PointTy> &a, const Point<PointTy> &b,
                              const Point<PointTy> &c) {
  Vector<PointTy> AB{b, a};
  Vector<PointTy> AC{c, a};
  Vector<PointTy> cross_res = cross(AB, AC);

  if (double_cmp(cross_res.x, 0.0) && double_cmp(cross_res.y, 0.0) &&
      double_cmp(cross_res.z, 0.0)) {
    return true;
  }

  return false;
}

template <typename PointTy = double>
bool is_point_in_triangle(const Triangle<PointTy> t, const Point<PointTy> p) {
  Vector<PointTy> v0{t.get_a(), t.get_b()};
  Vector<PointTy> v1{t.get_c(), t.get_a()};
  Vector<PointTy> v2{t.get_b(), t.get_c()};

  Vector<PointTy> PA = cross(v0, p - t.get_b());
  Vector<PointTy> PB = cross(v1, p - t.get_a());
  Vector<PointTy> PC = cross(v2, p - t.get_c());

  PA.normalize();
  PB.normalize();
  PC.normalize();

  Vector<PointTy> zero{0, 0, 0};

  if ((PA == zero && PB == zero) || (PB == zero && PC == zero) ||
      (PB == zero && PC == zero)) {
    return true;
  }

  if (PA == zero) {
    if (PB == PC)
      return true;
    return false;
  }

  if (PB == zero) {
    if (PA == PC)
      return true;
    return false;
  }

  if (PC == zero) {
    if (PA == PB)
      return true;
    return false;
  }

  if (PA == PB && PB == PC)
    return true;
  return false;
}
