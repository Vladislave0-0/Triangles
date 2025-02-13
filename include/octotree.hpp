#pragma once

#include <deque>
#include <map>
#include <math.h>
#include <vector>

#include "triangles.hpp"

namespace triangle {

template <typename PointTy = double> class BoundingBox {
  std::vector<Triangle<PointTy>> trg_in_cell;

  Vector<PointTy> min, max;

public:
  BoundingBox(const std::vector<Triangle<PointTy>> &triangles)
      : trg_in_cell(triangles) {
    auto it = trg_in_cell.begin();
    min.x = max.x = it->min_x();
    min.y = max.y = it->min_y();
    min.z = max.z = it->min_z();

    for (; it != trg_in_cell.end(); ++it) {
      max.x = std::max(max.x, it->max_x());
      min.x = std::min(min.x, it->min_x());
      max.y = std::max(max.y, it->max_y());
      min.y = std::min(min.y, it->min_y());
      max.z = std::max(max.z, it->max_z());
      min.z = std::min(min.z, it->min_z());
    }
  }

  PointTy average_x() const { return (max.x + min.x) / 2; }

  PointTy average_y() const { return (max.y + min.y) / 2; }

  PointTy average_z() const { return (max.z + min.z) / 2; }

  std::vector<Triangle<PointTy>> &get_trg_in_cell() { return trg_in_cell; }

  void group_intersections(std::map<size_t, size_t> &result) {
    for (auto one = trg_in_cell.begin(); one != trg_in_cell.end(); ++one) {
      auto it = one;
      it++;

      for (auto two = it; two != trg_in_cell.end(); ++two) {
        if (check_intersection(*one, *two)) {
          result[(*one).id] = (*one).id;
          result[(*two).id] = (*two).id;
        }
      }
    }
  }
};

template <typename PointTy = float> class Octotree {
  std::vector<Triangle<PointTy>> input;
  std::deque<BoundingBox<PointTy>> cells;

  size_t depth = 0;
  size_t cells_num = 0;
  size_t triag_num = 0;
  int axis = 0;

public:
  Octotree(const std::vector<Triangle<PointTy>> &triangles,
           const size_t triag_num)
      : input(triangles), triag_num(triag_num) {
    cells.push_back(BoundingBox<PointTy>(input));
    depth = count_depth(triag_num);
    ++cells_num;
  };

  size_t count_depth(const size_t &triag_num) const {
    if (triag_num < 1000) {
      return 0;
    }

    if (triag_num < 10000) {
      return 1;
    }

    if (triag_num < 100000) {
      return 2;
    }

    return 3;
  }

  const std::deque<BoundingBox<PointTy>> &get_cells() { return cells; }

  void divide_cell() {
    std::vector<Triangle<PointTy>> plus;
    std::vector<Triangle<PointTy>> minus;

    size_t copy_num_of_cells = cells_num;

    for (int i = 0; i < copy_num_of_cells; ++i) {
      auto front_groups = cells.front();

      size_t nod = axis % 3;
      PointTy average = calculate_average(front_groups, nod);

      for (const auto &it : front_groups.get_trg_in_cell()) {
        PointTy coordinates[3][3] = {it.get_a().x, it.get_b().x, it.get_c().x,
                                     it.get_a().y, it.get_b().y, it.get_c().y,
                                     it.get_a().z, it.get_b().z, it.get_c().z};

        if (coordinates[nod][0] >= average || coordinates[nod][1] >= average ||
            coordinates[nod][2] >= average) {
          plus.push_back(it);
        }

        if (coordinates[nod][0] <= average || coordinates[nod][1] <= average ||
            coordinates[nod][2] <= average) {
          minus.push_back(it);
        }
      }

      if (plus.size() + minus.size() <
          front_groups.get_trg_in_cell().size() * 2) {
        if (!plus.empty()) {
          cells.push_back(BoundingBox<PointTy>(plus));
          ++cells_num;
        }

        if (!minus.empty()) {
          cells.push_back(BoundingBox<PointTy>(minus));
          ++cells_num;
        }

        cells.pop_front();
        --cells_num;
      }

      plus.clear();
      minus.clear();
      ++axis;
    }
  }

  PointTy calculate_average(const BoundingBox<PointTy> &box, int nod) const {
    switch (nod) {
    case 0:
      return box.average_x();
    case 1:
      return box.average_y();
    case 2:
      return box.average_z();
    default:
      throw std::invalid_argument("Invalid axis");
    }
  }

  void divide_tree() {
    for (int i = 0; i < depth; ++i) {
      for (int i = 0; i < 3; ++i)
        divide_cell();
    }
  }
};
} // namespace triangle
