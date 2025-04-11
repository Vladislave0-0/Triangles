#pragma once

#include "config.hpp"
#include <iostream>

namespace triangle {

template <typename PointTy> class Triangle;
template <typename PointTy> class Vector;

template <typename PointTy = double> struct Point {
  PointTy x = NAN, y = NAN, z = NAN;

  void print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
  }

  void print_no_endl() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")";
  }

  PointTy norm() const { return x * x + y * y + z * z; }

  bool operator==(const Point<PointTy> &other) const = default;

  Vector<PointTy> operator-(const Point<PointTy> &other) const {
    return {x - other.x, y - other.y, z - other.z};
  }
};

template <typename PointTy = double> bool valid(const Point<PointTy> &point) {
  return !std::isnan(point.x) && !std::isnan(point.y) && !std::isnan(point.z);
}

template <typename PointTy = double>
bool equal(const Point<PointTy> &point1, const Point<PointTy> &point2) {
  if (!valid(point1) || !valid(point2))
    return false;

  return cmp(point1.x, point2.x) && cmp(point1.y, point2.y) &&
         cmp(point1.z, point2.z);
}

template <typename PointTy = double>
Point<PointTy> point_from_vector(const Vector<PointTy> &vector) {
  return Point{vector.x, vector.y, vector.z};
}

template <typename PointTy = double>
bool three_points_on_one_line(const Point<PointTy> &a, const Point<PointTy> &b,
                              const Point<PointTy> &c) {
  Vector<PointTy> cross_res = cross(Vector(b - a), Vector(c - a));

  if (cmp(cross_res.x, 0.0) && cmp(cross_res.y, 0.0) && cmp(cross_res.z, 0.0))
    return true;

  return false;
}

template <typename PointTy = double>
bool point_in_triangle(const Triangle<PointTy> t, const Point<PointTy> p) {
  Vector<PointTy> PA = cross(t.get_a() - t.get_b(), p - t.get_b());
  Vector<PointTy> PB = cross(t.get_c() - t.get_a(), p - t.get_a());
  Vector<PointTy> PC = cross(t.get_b() - t.get_c(), p - t.get_c());

  PA.normalize();
  PB.normalize();
  PC.normalize();

  Vector<PointTy> zero{0, 0, 0};

  if ((equal(PA, zero) && equal(PB, zero)) ||
      (equal(PA, zero) && equal(PC, zero)) ||
      (equal(PB, zero) && equal(PC, zero))) {
    return true;
  }

  if (equal(PA, zero) || equal(PB, zero) || equal(PC, zero)) {
    if (equal(PB, PC) || equal(PA, PC) || equal(PA, PB))
      return true;
    return false;
  }

  if (equal(PA, PB) && equal(PB, PC))
    return true;

  return false;
}
} // namespace triangle
