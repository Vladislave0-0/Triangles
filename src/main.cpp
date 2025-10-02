#include "octotree.hpp"
#include "visualizer/visualizer.hpp"

void print_help() {
    std::cout << "Usage: triag [OPTIONS] < input_file\n\n"
              << "Options:\n"
              << "  -v, --visualize   # Enable visualization mode\n"
              << "  -h, --help        # Show this help message\n"
              << "  --version         # Show version information\n\n"
              << "Examples:\n"
              << "  triag < input.txt          # Calculation mode (default)\n"
              << "  triag -v < input.txt       # Visualization mode with OpenGL\n";
}

int main(int argc, char **argv) {
  bool use_visualization = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      print_help();
      return 0;
    } else if (arg == "-v" || arg == "--visualize") {
      use_visualization = true;
    } else if (arg == "--version") {
      std::cout << "Triangles Intersection v2.0\n";
      return 0;
    } else {
      std::cerr << "Unknown option: " << arg << "\n";
      print_help();
      return 1;
    }
  }

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

  std::map<size_t, size_t> intersections;
  std::deque<BoundingBox<PointTy>> octotree_cells = octotree.get_cells();

  for (auto it : octotree_cells) {
    std::vector<Triangle<PointTy>> cur_cell = it.get_trg_in_cell();
    it.group_intersections(intersections);
  }

  if (argc > 1) {
    std::string arg = argv[1];
    use_visualization = (arg == "--visualize" || arg == "-v");
  }

  if (use_visualization) {
    visualizer::runVisualizer(input, intersections);
  } else {
    for (auto it = intersections.begin(); it != intersections.end(); ++it)
      std::cout << it->second << std::endl;
  }
}
