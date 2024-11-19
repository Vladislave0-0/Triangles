#pragma once

#include "point.hpp"
#include <math.h>

template <typename PointTy = double> class Interval {
  PointTy t0 = NAN;
  PointTy t1 = NAN;

public:
  Interval() = default;

  Interval(PointTy t0, PointTy t1) {
    this->t0 = std::max(t0, t1);
    this->t1 = std::min(t0, t1);
  }

  bool overlap(const Interval &another_interval) const;

  bool valid() const { return !std::isnan(t0) && !std::isnan(t1); }

  void print() const {
    std::cout << "[" << t0 << ", " << t1 << "]" << std::endl;
  }
};
