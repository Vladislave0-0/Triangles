#pragma once

#include "point.hpp"

template <typename PointTy = float> class Triangle {
  enum TriangleType { NONE, POINT, LINE, TRIANGLE };

  Point<PointTy> a, b, c;

  bool validate() {
    if (a == b && b == c) {
      type = POINT;
      return false;
    } else if (a == b || b == c || a == c) {
      type = LINE;
      return false;
    }

    type = TRIANGLE;
    return true;
  }

public:
  TriangleType type = NONE;

  Triangle(const PointTy &x1, const PointTy &y1, const PointTy &z1,
           const PointTy &x2, const PointTy &y2, const PointTy &z2,
           const PointTy &x3, const PointTy &y3, const PointTy &z3)
      : a(Point<PointTy>(x1, y1, z1)), b(Point<PointTy>(x2, y2, z2)),
        c(Point<PointTy>(x3, y3, z3)) {
    if (a.norm() <= b.norm()) {
      std::swap(a, b);
    }

    if (a.norm() <= c.norm()) {
      std::swap(a, c);
    }

    if (b.norm() <= c.norm()) {
      std::swap(b, c);
    }

    validate();
  }

  void print() const {
    a.print();
    b.print();
    c.print();
  }
};
