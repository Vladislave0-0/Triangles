#include "../include/octotree.hpp"

int main() {
  using namespace triangle;
  using PointTy = double;

  std::vector<Triangle<PointTy>> input;
  size_t triag_num = 0;
  std::cin >> triag_num;

  for (size_t i = 0; i < triag_num; ++i) {
    PointTy x1 = 0, y1 = 0, z1 = 0;
    PointTy x2 = 0, y2 = 0, z2 = 0;
    PointTy x3 = 0, y3 = 0, z3 = 0;

    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

    Triangle<PointTy> triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
    triangle.id = i;
    input.push_back(triangle);
  }

  Octotree<PointTy> octotree(input, calculate_octotree_depth(triag_num));
  octotree.divide_tree();

  std::map<size_t, size_t> result;
  std::deque<BoundingBox<PointTy>> octotree_cells = octotree.get_cells();

  for (auto it : octotree_cells) {
    std::vector<Triangle<PointTy>> cur_cell = it.get_trg_in_cell();
    it.group_intersections(result);
  }

  for (auto it = result.begin(); it != result.end(); ++it) {
    std::cout << it->second << std::endl;
  }
}
