#pragma once

#include <cmath>
#include <math.h>

#include "config.hpp"
#include "point.hpp"
#include "vector.hpp"

template <typename PointTy = double> class Plane {
  PointTy A, B, C, D = NAN;
  PointTy normal_length = NAN;
  Vector<PointTy> normal;

  void normalize_plane() {
    normal_length = std::sqrt(A * A + B * B + C * C);

    A = A / normal_length;
    B = B / normal_length;
    C = C / normal_length;
    D = D / normal_length;
  }

public:
  Plane(const Point<PointTy> &a, const Point<PointTy> &b,
        const Point<PointTy> &c) {
    A = (b.get_y() - a.get_y()) * (c.get_z() - a.get_z()) -
        (b.get_z() - a.get_z()) * (c.get_y() - a.get_y());
    B = (b.get_z() - a.get_z()) * (c.get_x() - a.get_x()) -
        (b.get_x() - a.get_x()) * (c.get_z() - a.get_z());
    C = (b.get_x() - a.get_x()) * (c.get_y() - a.get_y()) -
        (b.get_y() - a.get_y()) * (c.get_x() - a.get_x());
    D = -(A * a.get_x() + B * a.get_y() + C * a.get_z());

    normal.x = A;
    normal.y = B;
    normal.z = C;

    normalize_plane();
  }

  Plane(const PointTy &x1, const PointTy &y1, const PointTy &z1,
        const PointTy &x2, const PointTy &y2, const PointTy &z2,
        const PointTy &x3, const PointTy &y3, const PointTy &z3) {
    A = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
    B = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
    C = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    D = -(A * x1 + B * y1 + C * z1);

    normal.x = A;
    normal.y = B;
    normal.z = C;

    normalize_plane();
  }

  void print() const {
    std::cout << A << "x + " << B << "y + " << C << "z + " << D << " = 0"
              << std::endl;
  }

  void print_normal() const {
    std::cout << "(" << normal.x << ", " << normal.y << ", " << normal.z << ")"
              << std::endl;
  }

  PointTy substitute(const Point<PointTy> &point) const {
    return A * point.get_x() + B * point.get_y() + C * point.get_z() + D;
  }

  PointTy substitute(const PointTy &x1, const PointTy &y1,
                     const PointTy &z1) const {
    return A * x1 + B * y1 + C * z1 + D;
  }

  bool operator==(const Plane<PointTy> &other) const {
    // Проверка на совпадени плоскостей с точностью до знака
    if ((double_cmp(A, other.A) && double_cmp(B, other.B) &&
         double_cmp(C, other.C) && double_cmp(D, other.D)) ||
        (double_cmp(-A, other.A) && double_cmp(-B, other.B) &&
         double_cmp(-C, other.C) && double_cmp(-D, other.D))) {
      return true;
    }

    return false;
  }

  PointTy get_A() const { return A; }
  PointTy get_B() const { return B; }
  PointTy get_C() const { return C; }
  PointTy get_D() const { return D; }
  Vector<PointTy> get_normal() const { return normal; }
};

template <typename PointTy = double>
PointTy planes_are_parallel(const Plane<PointTy> &plane1,
                            const Plane<PointTy> &plane2) {
  if ((double_cmp(plane1.get_A(), plane2.get_A()) &&
       double_cmp(plane1.get_B(), plane2.get_B()) &&
       double_cmp(plane1.get_C(), plane2.get_C())) ||
      (double_cmp(-plane1.get_A(), plane2.get_A()) &&
       double_cmp(-plane1.get_B(), plane2.get_B()) &&
       double_cmp(-plane1.get_C(), plane2.get_C()))) {
    return true;
  }

  return false;
}

template <typename PointTy = double>
Vector<PointTy> get_planes_intersection_vector(const Plane<PointTy> &plane1,
                                               const Plane<PointTy> &plane2) {
  Vector vec = cross(plane1.get_normal(), plane2.get_normal());
  return vec;
}

template <typename PointTy = double>
Point<PointTy> get_planes_intersection_point(const Plane<PointTy> &plane1,
                                             const Plane<PointTy> &plane2) {
  PointTy x, y, z = NAN;

  PointTy determinant_x_zero =
      plane1.get_B() * plane2.get_C() - plane1.get_C() * plane2.get_B();
  PointTy determinant_y_zero =
      plane1.get_A() * plane2.get_C() - plane1.get_C() * plane2.get_A();
  PointTy determinant_z_zero =
      plane1.get_A() * plane2.get_B() - plane1.get_B() * plane2.get_A();

  // A1x + B1y = -D1
  // A2x + B2y = -D2
  if (!double_cmp(determinant_z_zero, 0.0)) {
    x = (plane1.get_B() * plane2.get_D() - plane2.get_B() * plane1.get_D()) /
        determinant_z_zero;
    y = (plane2.get_A() * plane1.get_D() - plane1.get_A() * plane2.get_D()) /
        determinant_z_zero;
    z = 0.0;
  }
  // A1x + С1z = -D1
  // A2x + С2z = -D2
  else if (!double_cmp(determinant_y_zero, 0.0)) {
    x = (plane1.get_C() * plane2.get_D() - plane2.get_C() * plane1.get_D()) /
        determinant_y_zero;
    z = (plane2.get_A() * plane1.get_D() - plane1.get_A() * plane2.get_D()) /
        determinant_y_zero;
    y = 0.0;
  }
  // B1y + С1z = -D1
  // B2y + С2z = -D2
  else if (!double_cmp(determinant_x_zero, 0.0)) {
    y = (plane1.get_C() * plane2.get_D() - plane2.get_C() * plane1.get_D()) /
        determinant_x_zero;
    z = (plane2.get_B() * plane1.get_D() - plane1.get_B() * plane2.get_D()) /
        determinant_x_zero;
    x = 0.0;
  }

  Point<PointTy> intersectionPoint = {x, y, z};
  return intersectionPoint;
}
