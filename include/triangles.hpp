#pragma once

#include "interval.hpp"
#include "line.hpp"
#include "plane.hpp"

namespace triangle {

template <typename PointTy = double> class Triangle {
public:
  enum TriangleType { NONE, POINT, LINE, TRIANGLE };
  size_t id = 0;

private:
  TriangleType type = NONE;

  Point<PointTy> a, b, c;

  void validate() {
    if (!valid(a) || !valid(b) || !valid(c))
      return;

    if (equal(a, b) && equal(b, c)) {
      type = POINT;
      return;
    } else if (equal(a, b) || equal(b, c) || equal(a, c) ||
               three_points_on_one_line(a, b, c)) {
      type = LINE;
      return;
    }

    type = TRIANGLE;
  }

public:
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

  Triangle(const Point<PointTy> &p1, const Point<PointTy> &p2,
           const Point<PointTy> &p3)
      : a(p1), b(p2), c(p3) {
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

  Point<PointTy> get_a() const { return a; }

  Point<PointTy> get_b() const { return b; }

  Point<PointTy> get_c() const { return c; }

  TriangleType get_type() const { return type; }

  PointTy min_x() const { return std::min(a.x, std::min(b.x, c.x)); }

  PointTy max_x() const { return std::max(a.x, std::max(b.x, c.x)); }

  PointTy min_y() const { return std::min(a.y, std::min(b.y, c.y)); }

  PointTy max_y() const { return std::max(a.y, std::max(b.y, c.y)); }

  PointTy min_z() const { return std::min(a.z, std::min(b.z, c.z)); }

  PointTy max_z() const { return std::max(a.z, std::max(b.z, c.z)); }
};

template <typename PointTy = double>
bool check_intersection(Triangle<PointTy> &t1, Triangle<PointTy> &t2) {
  using TYPE = typename Triangle<PointTy>::TriangleType;

  TYPE type1 = t1.get_type();
  TYPE type2 = t2.get_type();

  if (type1 == TYPE::TRIANGLE && type2 == TYPE::TRIANGLE) {
    return intersect_triangle_with_triangle_in_3D(t1, t2);
  }

  if (type1 == TYPE::TRIANGLE && type2 == TYPE::LINE) {
    return intersect_triangle_with_line_in_3D(t1, t2);
  } else if (type1 == TYPE::LINE && type2 == TYPE::TRIANGLE) {
    return intersect_triangle_with_line_in_3D(t2, t1);
  }

  if (type1 == TYPE::TRIANGLE && type2 == TYPE::POINT) {
    return intersect_triangle_with_point(t1, t2);
  } else if (type1 == TYPE::POINT && type2 == TYPE::TRIANGLE) {
    return intersect_triangle_with_point(t2, t1);
  }

  if (type1 == TYPE::LINE && type2 == TYPE::LINE) {
    return intersect_line_with_line(t1, t2);
  }

  if (type1 == TYPE::LINE && type2 == TYPE::POINT) {
    return intersect_line_with_point(t1, t2);
  } else if (type1 == TYPE::POINT && type2 == TYPE::LINE) {
    return intersect_line_with_point(t2, t1);
  }

  if (type1 == TYPE::POINT && type2 == TYPE::POINT) {
    return equal(t1.get_a(), t2.get_a());
  }

  return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_triangle_in_3D(Triangle<PointTy> &t1,
                                            Triangle<PointTy> &t2) {
  Plane<PointTy> plane1(t1.get_a(), t1.get_b(), t1.get_c());
  Plane<PointTy> plane2(t2.get_a(), t2.get_b(), t2.get_c());

  // Checking for plane alignment.
  if (planes_are_parallel(plane1, plane2)) {
    if ((plane1.get_D() == plane2.get_D()) ||
        (-plane1.get_D() == plane2.get_D())) {
      return intersect_triangle_with_triangle_in_2D(t1, t2);
    }

    return false;
  }

  // If all the sign distances from the vertices of triangle T2 to triangle T1
  // are of the same sign, then the triangles do not intersect.
  PointTy signed_dist11 = plane1.substitute(t2.get_a());
  PointTy signed_dist21 = plane1.substitute(t2.get_b());
  PointTy signed_dist31 = plane1.substitute(t2.get_c());
  if ((signed_dist11 < 0 && signed_dist21 < 0 && signed_dist31 < 0) ||
      (signed_dist11 > 0 && signed_dist21 > 0 && signed_dist31 > 0)) {
    return false;
  }

  // If all the sign distances from the vertices of triangle T1 to triangle T2
  // are of the same sign, then the triangles do not intersect.
  PointTy signed_dist12 = plane2.substitute(t1.get_a());
  PointTy signed_dist22 = plane2.substitute(t1.get_b());
  PointTy signed_dist32 = plane2.substitute(t1.get_c());
  if ((signed_dist12 < 0 && signed_dist22 < 0 && signed_dist32 < 0) ||
      (signed_dist12 > 0 && signed_dist22 > 0 && signed_dist32 > 0)) {
    return false;
  }

  // Then let's check their intersection. Let's find the line of intersection of
  // two planes.
  Line<PointTy> inter_line{get_planes_intersection_vector(plane1, plane2),
                           get_planes_intersection_point(plane1, plane2)};

  // Let's find the intervals of intersection of triangles with the line of
  // intersection of planes.
  Interval interval1 = get_interval_of_triangle_and_line(inter_line, t1);
  Interval interval2 = get_interval_of_triangle_and_line(inter_line, t2);

  if (!interval1.valid() || !interval2.valid())
    return false;

  // Let's check if the intervals intersect.
  return intersect_intervals(interval1, interval2);
}

template <typename PointTy = double>
Interval<PointTy>
get_interval_of_triangle_and_line(const Line<PointTy> &inter_line,
                                  const Triangle<PointTy> &triangle) {
  Line<PointTy> line1{triangle.get_b() - triangle.get_a(), triangle.get_a()};
  Line<PointTy> line2{triangle.get_c() - triangle.get_a(), triangle.get_a()};
  Line<PointTy> line3{triangle.get_c() - triangle.get_b(), triangle.get_b()};

  Point<PointTy> inter_point1 = intersect_line_with_line(inter_line, line1);
  Point<PointTy> inter_point2 = intersect_line_with_line(inter_line, line2);
  Point<PointTy> inter_point3 = intersect_line_with_line(inter_line, line3);

  Interval interval = get_valid_interval_of_triangle_and_line(
      triangle, inter_point1, inter_point2, inter_point3);

  return interval;
}

template <typename PointTy = double>
bool intersect_triangle_with_triangle_in_2D(Triangle<PointTy> &t1,
                                            Triangle<PointTy> &t2) {
  Line<PointTy> line1{t2.get_b() - t2.get_a(), t2.get_a()};
  Line<PointTy> line2{t2.get_c() - t2.get_a(), t2.get_a()};
  Line<PointTy> line3{t2.get_c() - t2.get_b(), t2.get_b()};

  Point<PointTy> a = t2.get_a();
  Point<PointTy> b = t2.get_b();
  Point<PointTy> c = t2.get_c();

  if (point_in_triangle(t1, a) || point_in_triangle(t1, b) ||
      point_in_triangle(t1, c)) {
    return true;
  }

  if (point_in_triangle(t2, t1.get_a()) || point_in_triangle(t2, t1.get_b()) ||
      point_in_triangle(t2, t1.get_c())) {
    return true;
  }

  if (intersect_triangle_with_segment_in_2D(t1, line1, a, b) ||
      intersect_triangle_with_segment_in_2D(t1, line2, a, c) ||
      intersect_triangle_with_segment_in_2D(t1, line3, b, c)) {
    return true;
  }

  return false;
}

template <typename PointTy = double>
bool intersect_triangle_with_line_in_3D(const Triangle<PointTy> &t1,
                                        const Triangle<PointTy> &t2) {
  Line<PointTy> line = get_line_from_triangle(t2);

  Vector<PointTy> e1 = t1.get_b() - t1.get_a();
  Vector<PointTy> e2 = t1.get_c() - t1.get_a();

  Vector<PointTy> p = cross(line.vector, e2);
  PointTy det = dot(p, e1);

  if (cmp(det, 0.0)) {
    return intersect_triangle_with_segment_in_2D(
        t1, line, get_triangle_space(t2).first, get_triangle_space(t2).second);
  }

  Vector<PointTy> s = line.point - t1.get_a();
  PointTy u = dot(s, p) / det;
  if (u < 0.0 || u > 1.0)
    return false;

  Vector<PointTy> q = cross(s, e1);
  PointTy v = dot(line.vector, q) / det;
  if (v < 0.0 || v > 1.0)
    return false;

  PointTy t = dot(e2, q) / det;
  if (t < 0.0 || t > 1.0)
    return false;

  if ((u + v) > 1.0)
    return false;

  return true;
}

template <typename PointTy = double>
bool intersect_triangle_with_point(const Triangle<PointTy> t1,
                                   const Triangle<PointTy> t2) {
  return point_in_triangle(t1, t2.get_a());
}

template <typename PointTy = double>
bool check_segments_intersection(const PointTy &min1, const PointTy &max1,
                                 const PointTy &min2, const PointTy &max2) {
  if (max1 < min2 || min1 > max2)
    return false;

  if (cmp(min1, min2) && cmp(min1, max1) && cmp(min1, max2))
    return false;

  return true;
}

template <typename PointTy = double>
bool intersect_line_with_line(const Triangle<PointTy> t1,
                              const Triangle<PointTy> t2) {
  Line<PointTy> line1 = get_line_from_triangle(t1);
  Line<PointTy> line2 = get_line_from_triangle(t2);

  Point<PointTy> point = intersect_line_with_line(line1, line2);

  auto [min1, max1] = get_triangle_space(t1);
  auto [min2, max2] = get_triangle_space(t2);

  if (!valid(point)) {
    if (line1 == line2) {
      if (check_segments_intersection(min1.x, max1.x, min2.x, max2.x) ||
          check_segments_intersection(min1.y, max1.y, min2.y, max2.y) ||
          check_segments_intersection(min1.z, max1.z, min2.z, max2.z)) {
        return true;
      }
    }
    
    return false;
  }

  return inter_point_inside_space(min1, max1, min2, max2, point);
}

template <typename PointTy = double>
bool inter_point_on_sides(const Point<PointTy> &a1, const Point<PointTy> &b1,
                          const Point<PointTy> &a2, const Point<PointTy> &b2,
                          const Point<PointTy> &point) {
  if (!valid(point))
    return false;

  auto [min1, max1] = get_segment_space(a1, b1);
  auto [min2, max2] = get_segment_space(a2, b2);

  return inter_point_inside_space(min1, max1, min2, max2, point);
}

template <typename PointTy = double>
bool inter_point_inside_space(const Point<PointTy> &min1,
                              const Point<PointTy> &max1,
                              const Point<PointTy> &min2,
                              const Point<PointTy> &max2,
                              const Point<PointTy> &point) {
  auto is_within_bounds = [](const Point<PointTy> &min,
                             const Point<PointTy> &max,
                             const Point<PointTy> &pt) {
    return (pt.x >= min.x && pt.x <= max.x ||
            (cmp(pt.x, min.x) && cmp(pt.x, max.x))) &&
           (pt.y >= min.y && pt.y <= max.y ||
            (cmp(pt.y, min.y) && cmp(pt.y, max.y))) &&
           (pt.z >= min.z && pt.z <= max.z ||
            (cmp(pt.z, min.z) && cmp(pt.z, max.z)));
  };

  return is_within_bounds(min1, max1, point) &&
         is_within_bounds(min2, max2, point);
}

template <typename PointTy = double>
bool intersect_triangle_with_segment_in_2D(const Triangle<PointTy> &t,
                                           const Line<PointTy> &line,
                                           const Point<PointTy> &seg_min,
                                           const Point<PointTy> &seg_max) {
  Line<PointTy> line1{t.get_b() - t.get_a(), t.get_a()};
  Point<PointTy> point1 = intersect_line_with_line(line, line1);
  if (inter_point_on_sides(t.get_a(), t.get_b(), seg_min, seg_max, point1))
    return true;

  Line<PointTy> line2{t.get_c() - t.get_a(), t.get_a()};
  Point<PointTy> point2 = intersect_line_with_line(line, line2);
  if (inter_point_on_sides(t.get_a(), t.get_c(), seg_min, seg_max, point2))
    return true;

  Line<PointTy> line3{t.get_c() - t.get_b(), t.get_b()};
  Point<PointTy> point3 = intersect_line_with_line(line, line3);
  if (inter_point_on_sides(t.get_b(), t.get_c(), seg_min, seg_max, point3))
    return true;

  return false;
}

template <typename PointTy = double>
std::pair<Point<PointTy>, Point<PointTy>>
get_triangle_space(const Triangle<PointTy> t) {
  PointTy min_x = std::min(t.get_a().x, std::min(t.get_b().x, t.get_c().x));
  PointTy max_x = std::max(t.get_a().x, std::max(t.get_b().x, t.get_c().x));
  PointTy min_y = std::min(t.get_a().y, std::min(t.get_b().y, t.get_c().y));
  PointTy max_y = std::max(t.get_a().y, std::max(t.get_b().y, t.get_c().y));
  PointTy min_z = std::min(t.get_a().z, std::min(t.get_b().z, t.get_c().z));
  PointTy max_z = std::max(t.get_a().z, std::max(t.get_b().z, t.get_c().z));

  return std::make_pair(Point(min_x, min_y, min_z), Point(max_x, max_y, max_z));
}

template <typename PointTy = double>
std::pair<Point<PointTy>, Point<PointTy>>
get_segment_space(const Point<PointTy> a, const Point<PointTy> b) {
  PointTy min_x = std::min(a.x, b.x);
  PointTy max_x = std::max(a.x, b.x);
  PointTy min_y = std::min(a.y, b.y);
  PointTy max_y = std::max(a.y, b.y);
  PointTy min_z = std::min(a.z, b.z);
  PointTy max_z = std::max(a.z, b.z);

  return std::make_pair(Point(min_x, min_y, min_z), Point(max_x, max_y, max_z));
}
} // namespace triangle
