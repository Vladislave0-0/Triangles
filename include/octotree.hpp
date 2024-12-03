#pragma once

#include "triangles.hpp"
#include "vector.hpp"
#include <list>
#include <vector>

template <typename PointTy> class Triangle;

template <typename PointTy = double> struct BoundingBox {
  Vector<PointTy> min;
  Vector<PointTy> max;
  BoundingBox() : min{0, 0, 0}, max{0, 0, 0} {};
  BoundingBox(Vector<PointTy> min, Vector<PointTy> max) : min{min}, max{max} {};

  bool contains(const Point<PointTy> &p) const {
    return (p.get_x() >= min.x) && (p.get_y() >= min.y) &&
           (p.get_z() >= min.z) && (p.get_x() <= max.x) &&
           (p.get_y() <= max.y) && (p.get_z() <= max.z);
  }

  bool contains(const Triangle<PointTy> &t) const {
    return contains(t.get_a()) && contains(t.get_b()) && contains(t.get_c());
  }
};

template <typename PointTy = double> class OctTree {
public:
  static constexpr int MIN_SIZE = 1;

  BoundingBox<PointTy> region;
  std::list<Triangle<PointTy>> region_elems;

  std::vector<OctTree> childNodes;
  OctTree *parent;

  OctTree() = default;
  OctTree(const BoundingBox<PointTy> &region,
          const std::list<Triangle<PointTy>> &region_elems)
      : region{region}, region_elems{region_elems} {};

  void BuildTree() {
    if (region_elems.size() <= 1)
      return;

    Vector<PointTy> dimensions = region.max - region.min;

    if (dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE &&
        dimensions.z <= MIN_SIZE)
      return;

    Vector<PointTy> half = dimensions / 2.0;
    Vector<PointTy> center = region.min + half;

    BoundingBox<PointTy> *octant = new BoundingBox<PointTy>[8];

    octant[0] = BoundingBox(region.min, center);
    octant[1] =
        BoundingBox(Vector<PointTy>(center.x, region.min.y, region.min.z),
                    Vector<PointTy>(region.max.x, center.y, center.z));

    octant[2] =
        BoundingBox(Vector<PointTy>(center.x, region.min.y, center.z),
                    Vector<PointTy>(region.max.x, center.y, region.max.z));

    octant[3] =
        BoundingBox(Vector<PointTy>(region.min.x, region.min.y, center.z),
                    Vector<PointTy>(center.x, center.y, region.max.z));

    octant[4] =
        BoundingBox(Vector<PointTy>(region.min.x, center.y, region.min.z),
                    Vector<PointTy>(center.x, region.max.y, center.z));

    octant[5] =
        BoundingBox(Vector<PointTy>(center.x, center.y, region.min.z),
                    Vector<PointTy>(region.max.x, region.max.y, center.z));

    octant[6] = BoundingBox(center, region.max);

    octant[7] =
        BoundingBox(Vector<PointTy>(region.min.x, center.y, center.z),
                    Vector<PointTy>(center.x, region.max.y, region.max.z));

    std::list<Triangle<PointTy>> *octList = new std::list<Triangle<PointTy>>[8];

    using list_iter = typename std::list<Triangle<PointTy>>::iterator;

    list_iter it = region_elems.begin();
    std::list<list_iter> to_delete;

    while (it != region_elems.end()) {
      Triangle<PointTy> obj = *it;
      if (obj.get_type() == Triangle<PointTy>::TRIANGLE) {
        for (int a = 0; a < 8; ++a) {
          if (octant[a].contains(obj)) {
            octList[a].push_back(obj);
            to_delete.push_back(it);
            break;
          }
        }
      }
      it++;
    }

    for (auto elem : to_delete)
      region_elems.erase(elem);

    childNodes = std::vector<OctTree>(8);

    // Создаём дочерние узлы, в которых находятся элементы, содержащиеся в
    // ограничивающей области
    for (int a = 0; a < 8; ++a) {
      if (octList[a].size() != 0) {
        childNodes[a] = OctTree(octant[a], octList[a]);
        childNodes[a].parent = this;
        childNodes[a].BuildTree();
      }
    }
  }

  std::list<int>
  getTreeIntersectNum(std::list<Triangle<PointTy>> parentObjs) const {

    std::list<int> intersects;

    for (auto it1 = region_elems.begin(); it1 != region_elems.end(); ++it1) {
      bool is_added = false;
      for (auto it2 = parentObjs.begin(); it2 != parentObjs.end(); ++it2) {
        Triangle<PointTy> tr1 = *it1;
        Triangle<PointTy> tr2 = *it2;

        if (check_intersection(tr1, tr2)) {
          intersects.push_back(tr2.id);

          if (!is_added) {
            is_added = true;
            intersects.push_back(tr1.id);
          }
        }
      }
    }

    for (auto it1 = region_elems.begin(); it1 != region_elems.end(); ++it1) {
      bool is_added = false;
      for (auto it2 = std::next(it1); it2 != region_elems.end(); ++it2) {
        Triangle<PointTy> tr1 = *it1;
        Triangle<PointTy> tr2 = *it2;

        if (check_intersection(tr1, tr2)) {
          intersects.push_back(tr2.id);

          if (!is_added) {
            is_added = true;
            intersects.push_back(tr1.id);
          }
        }
      }
    }

    for (auto child : childNodes) {
      parentObjs.insert(parentObjs.end(), region_elems.begin(),
                        region_elems.end());
      intersects.unique();
      intersects.sort();
      intersects.merge(child.getTreeIntersectNum(parentObjs));
    }

    return intersects;
  }

  std::list<int> getAllTreeIntersectNum() const {
    std::list<Triangle<PointTy>> empty;
    std::list<int> ret = getTreeIntersectNum(std::list<Triangle<PointTy>>());
    ret.unique();

    return ret;
  }
};
