#include <gtest/gtest.h>

#include "../include/config.hpp"
#include "../include/triangles.hpp"

bool double_cmp(double x, double y) { return fabs(x - y) < _epsilon; }

TEST(TriangleWithTriangle, Intersection2D_1) {
  Point t1p1{0.4, -5.0, 0.0};
  Point t1p2{1.0, 2.0, 0.0};
  Point t1p3{5.0, 0.0, 0.0};
  Point t2p1{-1.0, 0.0, 0.0};
  Point t2p2{3.0, 2.0, 0.0};
  Point t2p3{6.0, 2.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_2) {
  Point t1p1{1.0, 0.0, 1.0};
  Point t1p2{1.0, 0.0, 5.0};
  Point t1p3{5.0, 0.0, 4.0};
  Point t2p1{5.0, 0.0, 2.0};
  Point t2p2{2.1, 0.0, 8.0};
  Point t2p3{1.0, 0.0, -1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_3) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.5, 0.0};
  Point t2p2{0.0, 1.5, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_4) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 1.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_5) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 1.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection2D_6) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 1.1, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.5, 1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_1) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 1.0};
  Point t2p3{-2.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_2) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 1.0};
  Point t2p3{-2.0, 2.0, 20.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_3) {
  Point t1p1{-1.0, 5.0, 4.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 1.0};
  Point t2p3{-2.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_4) {
  Point t1p1{-1.0, 5.0, 4.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{-1.0, -2.0, -1.0};
  Point t2p2{-1.0, -1.0, -1.0};
  Point t2p3{-2.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_5) {
  Point t1p1{2.0, -3.0, 7.0};
  Point t1p2{1.0, 4.0, 6.0};
  Point t1p3{5.0, 3.0, -1.0};
  Point t2p1{1.0, 1.0, 1.0};
  Point t2p2{1.0, 0.0, 1.0};
  Point t2p3{1.0, -1.0, 3.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_6) {
  Point t1p1{2.0, -3.0, 4.0};
  Point t1p2{1.0, 4.0, 6.0};
  Point t1p3{5.0, 3.0, -1.0};
  Point t2p1{-1.0, 5.0, 2.0};
  Point t2p2{2.0, -1.0, 4.0};
  Point t2p3{5.0, 0.0, 5.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_7) {
  Point t1p1{2.5, 5.234, -4.234};
  Point t1p2{1.31, 5.01, -1.14};
  Point t1p3{-1.1, -2.4, 5.0};
  Point t2p1{1.5, 2.1, 3.1};
  Point t2p2{-1.5, -5.1, 2.0};
  Point t2p3{4.1, 5.0, 3.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_8) {
  Point t1p1{2.2, 5.3, -4.2};
  Point t1p2{1.0, 1.0, -1.0};
  Point t1p3{2.0, 2.0, 2.0};
  Point t2p1{5.0, 5.0, 0.0};
  Point t2p2{1.0, 1.0, 1.0};
  Point t2p3{-3.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_9) {
  Point t1p1{2.2, 5.3, -4.2};
  Point t1p2{1.0, 1.0, -1.0};
  Point t1p3{2.0, 2.0, 2.0};
  Point t2p1{5.0, 5.0, 0.0};
  Point t2p2{2.0, 2.0, 2.0};
  Point t2p3{-3.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_10) {
  Point t1p1{2.2, 5.3, -4.2};
  Point t1p2{1.0, 1.0, -1.0};
  Point t1p3{-2.0, -2.0, -2.0};
  Point t2p1{5.0, 5.0, 0.0};
  Point t2p2{1.0, 1.0, -1.0};
  Point t2p3{-3.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_11) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{3.0, 4.0, 9.0};
  Point t2p2{9.0, 4.0, 16.0};
  Point t2p3{16.0, 4.0, 6.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_12) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{5.0, 16.0, 16.0};
  Point t2p2{19.0, 3.0, 16.0};
  Point t2p3{3.0, 2.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_13) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{2.0, 17.0, 18.0};
  Point t2p2{9.0, 20.0, 3.0};
  Point t2p3{14.0, 6.0, 14.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_14) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{12.0, 7.0, 6.0};
  Point t2p2{12.0, 16.0, 7.0};
  Point t2p3{13.0, 15.0, 11.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_15) {
  Point t1p1{18.0, 1.0, 20.0};
  Point t1p2{9.0, 7.0, 1.0};
  Point t1p3{3.0, 19.0, 6.0};
  Point t2p1{12.0, 4.0, 10.0};
  Point t2p2{17.0, 20.0, 13.0};
  Point t2p3{18.0, 10.0, 6.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_16) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{0.0, 1.0, 0.0};
  Point t1p3{0.0, 0.0, 1.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{5.0, 5.0, 1.0};
  Point t2p3{5.0, 5.0, 10.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_17) {
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{5.0, 5.0, 1.0};
  Point t2p3{5.0, 5.0, 10.0};
  Point t1p1{0.5, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 0.0};
  Point t1p3{0.0, 0.0, 0.5};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_18) {
  Point t1p1{5.0, 3.0, 2.0};
  Point t1p2{1.0, 1.0, 0.0};
  Point t1p3{-2.0, 3.0, 1.0};
  Point t2p1{4.0, 1.0, 4.0};
  Point t2p2{4.0, 7.0, 1.0};
  Point t2p3{3.0, 5.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithTriangle, Intersection3D_19) {
  Point t1p1{5.0, 3.0, 2.0};
  Point t1p2{1.0, 1.0, 0.0};
  Point t1p3{-2.0, 3.0, 1.0};
  Point t2p1{4.0, 1.0, 4.0};
  Point t2p2{3.0, 5.0, 0.0};
  Point t2p3{2.0, 3.0, -1.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_triangle_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection_1) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.0, 0.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection_2) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.0, 2.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
}

TEST(TriangleWithLine, Intersection_3) {
  Point t1p1{0.0, 0.0, 0.0};
  Point t1p2{0.0, 0.0, 2.0};
  Point t1p3{2.0, 0.0, 0.0};
  Point t2p1{0.0, 2.0, 0.0};
  Point t2p2{0.0, 2.0, 0.0};
  Point t2p3{0.0, 0.0, 3.0};
  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_line_in_3D(t1, t2));
}


// TEST(TriangleWithLine, Intersection_1) {
//   Point t1p1{0.0, 0.0, 0.0};
//   Point t1p2{1.0, 1.0, 1.0};
//   Point t1p3{2.0, 2.0, 2.0};
//   Point t2p1{3.0, 3.0, 3.0};
//   Point t2p2{4.0, 4.0, 4.0};
//   Point t2p3{2.0, 1.0, 0.0};
//   Triangle t1{t1p1, t1p2, t1p3};
//   Triangle t2{t2p1, t2p2, t2p3};

//   ASSERT_FALSE(intersect_triangle_with_line_in_3D(t1, t2));
// }

// TEST(TriangleWithLine, Intersection_2) {
//   Point t1p1{0.0, 0.0, 0.0};
//   Point t1p2{0.0, 2.0, 0.0};
//   Point t1p3{0.0, 0.0, 0.0};
//   Point t2p1{0.0, 0.0, 0.0};
//   Point t2p2{0.0, 2.0, 0.0};
//   Point t2p3{2.0, 0.0, 0.0};
//   Triangle t1{t1p1, t1p2, t1p3};
//   Triangle t2{t2p1, t2p2, t2p3};

//   ASSERT_TRUE(intersect_triangle_with_line_in_3D(t1, t2));
// }

// TEST(TriangleWithLine, Intersection_3) {
//   Point t1p1{0.0, 0.0, 0.0};
//   Point t1p2{0.0, 2.0, 0.0};
//   Point t1p3{0.0, 0.0, 0.0};
//   Point t2p1{3.0, 0.0, 0.0};
//   Point t2p2{5.0, 2.0, 0.0};
//   Point t2p3{6.0, 0.0, 0.0};
//   Triangle t1{t1p1, t1p2, t1p3};
//   Triangle t2{t2p1, t2p2, t2p3};

//   ASSERT_FALSE(intersect_triangle_with_line(t1, t2));
// }

// TEST(TriangleWithLine, Intersection_4) {
//   Point t1p1{-1.0, 0.0, 0.0};
//   Point t1p2{3.0, 2.0, 0.0};
//   Point t1p3{6.0, 0.0, 0.0};
//   Point t2p1{0.0, 0.0, 0.0};
//   Point t2p2{0.0, 2.0, 0.0};
//   Point t2p3{0.0, 0.0, 0.0};

//   Triangle t1{t1p1, t1p2, t1p3};
//   Triangle t2{t2p1, t2p2, t2p3};

//   ASSERT_TRUE(intersect_triangle_with_line(t1, t2));
// }

// TEST(TriangleWithLine, Intersection_5) {
//   Point t1p1{-1.0, 0.0, 0.0};
//   Point t1p2{3.0, 2.0, 0.0};
//   Point t1p3{6.0, 2.0, 0.0};
//   Point t2p1{0.0, 0.0, 0.0};
//   Point t2p2{0.0, 2.0, 0.0};
//   Point t2p3{0.0, 0.0, 0.0};

//   Triangle t1{t1p1, t1p2, t1p3};
//   Triangle t2{t2p1, t2p2, t2p3};

//   ASSERT_TRUE(intersect_triangle_with_line(t1, t2));
// }

TEST(TriangleWithPoint, Intersection_1) {
  Point t1p1{-0.5, 0.0, 2.0};
  Point t1p2{-0.5, 0.0, 0.0};
  Point t1p3{1.0, 0.0, 0.0};
  Point t2p1{-0.5, 0.0, 1.0};
  Point t2p2{-0.5, 0.0, 1.0};
  Point t2p3{-0.5, 0.0, 1.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_point(t1, t2));
}

TEST(TriangleWithPoint, Intersection_2) {
  Point t1p1{-0.5, 0.0, 2.0};
  Point t1p2{-0.5, 0.0, 0.0};
  Point t1p3{1.0, 0.0, 0.0};
  Point t2p1{-0.5, 0.0, 2.0};
  Point t2p2{-0.5, 0.0, 2.0};
  Point t2p3{-0.5, 0.0, 2.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_point(t1, t2));
}

TEST(TriangleWithPoint, Intersection_3) {
  Point t1p1{-0.5, 0.0, 2.0};
  Point t1p2{-0.5, 0.0, 0.0};
  Point t1p3{1.0, 0.0, 0.0};
  Point t2p1{0.0, 0.0, 1.0};
  Point t2p2{0.0, 0.0, 1.0};
  Point t2p3{0.0, 0.0, 1.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_triangle_with_point(t1, t2));
}

TEST(TriangleWithPoint, Intersection_4) {
  Point t1p1{-0.5, 0.0, 2.0};
  Point t1p2{-0.5, 0.0, 0.0};
  Point t1p3{1.0, 0.0, 0.0};
  Point t2p1{0.0, 0.0, 2.0};
  Point t2p2{0.0, 0.0, 2.0};
  Point t2p3{0.0, 0.0, 2.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_triangle_with_point(t1, t2));
}

TEST(LineWithPoint, Intersection_1) {
  Point t1p1{1.0, 1.0, 1.0};
  Point t1p2{2.0, 2.0, 2.0};
  Point t1p3{3.0, 3.0, 3.0};
  Point t2p1{0.5, 0.5, 0.5};
  Point t2p2{0.5, 0.5, 0.5};
  Point t2p3{0.5, 0.5, 0.5};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_point(t1, t2));
}

TEST(LineWithPoint, Intersection_2) {
  Point t1p1{1.0, 1.0, 1.0};
  Point t1p2{2.0, 2.0, 2.0};
  Point t1p3{2.0, 2.0, 2.0};
  Point t2p1{0.5, 0.5, 0.5};
  Point t2p2{0.5, 0.5, 0.5};
  Point t2p3{0.5, 0.5, 0.5};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_point(t1, t2));
}

TEST(LineWithPoint, Intersection_3) {
  Point t1p1{1.0, 1.0, 1.0};
  Point t1p2{2.0, 2.0, 2.0};
  Point t1p3{3.0, 3.0, 3.0};
  Point t2p1{0.0, 0.0, 1.0};
  Point t2p2{0.0, 0.0, 1.0};
  Point t2p3{0.0, 0.0, 1.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_line_with_point(t1, t2));
}

TEST(LineWithLine, Intersection_1) {
  Point t1p1{0.0, 1.0, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_2) {
  Point t1p1{0.0, 1.0, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_3) {
  Point t1p1{1.0, 0.0, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_4) {
  Point t1p1{0.5, 0.5, 0.0};
  Point t1p2{-0.5, 1.5, 0.0};
  Point t1p3{-0.5, 1.5, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_5) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{1.0, 1.0, 0.0};
  Point t2p3{1.0, 1.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_FALSE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_6) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{2.0, 2.0, 0.0};
  Point t2p3{2.0, 2.0, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_line(t1, t2));
}

TEST(LineWithLine, Intersection_7) {
  Point t1p1{2.0, 2.0, 0.0};
  Point t1p2{3.0, 3.0, 0.0};
  Point t1p3{3.0, 3.0, 0.0};
  Point t2p1{0.0, 0.0, 0.0};
  Point t2p2{2.1, 2.1, 0.0};
  Point t2p3{2.1, 2.1, 0.0};

  Triangle t1{t1p1, t1p2, t1p3};
  Triangle t2{t2p1, t2p2, t2p3};

  ASSERT_TRUE(intersect_line_with_line(t1, t2));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// TEST(SignedDistTest, Test1) {
//   Plane<double> plane1(0, 0, 0, 1, 0, 0, 0, 1, 0);
//   Plane<double> plane2(2, 1, 2.5, 2, 0, 1.5, 2, 1, 0.5);

//   double signed_dist1 = plane1.substitute(2, 1, 2.5);
//   double signed_dist2 = plane1.substitute(2, 0, 1.5);
//   double signed_dist3 = plane1.substitute(2, 1, 0.5);

//   bool result = true;
//   if ((signed_dist1 < 0 && signed_dist2 < 0 && signed_dist3 < 0) ||
//       (signed_dist1 > 0 && signed_dist2 > 0 && signed_dist3 > 0)) {
//     result = false;
//   }

//   EXPECT_FALSE(result);
// }

// TEST(SignedDistTest, Test2) {
//   Plane<double> plane1(0, 0, 0, 1, 0, 0, 0, 1, 0);
//   Plane<double> plane2(2, 0, -1, 2, 0, 1, 2, 1, 0);

//   double signed_dist11 = plane1.substitute(2, 1, -1);
//   double signed_dist12 = plane1.substitute(2, 0, 1);
//   double signed_dist13 = plane1.substitute(2, 1, 0);
//   double signed_dist21 = plane2.substitute(0, 0, 0);
//   double signed_dist22 = plane2.substitute(1, 0, 0);
//   double signed_dist23 = plane2.substitute(0, 1, 0);

//   bool result1 = true;
//   bool result2 = true;
//   bool result;

//   if ((signed_dist11 < 0 && signed_dist12 < 0 && signed_dist13 < 0) ||
//       (signed_dist11 > 0 && signed_dist12 > 0 && signed_dist13 > 0)) {
//     result1 = false;
//   }
//   if ((signed_dist21 < 0 && signed_dist22 < 0 && signed_dist23 < 0) ||
//       (signed_dist21 > 0 && signed_dist22 > 0 && signed_dist23 > 0)) {
//     result2 = false;
//   }

//   result = (result1 == true) && (result2 == false);

//   EXPECT_TRUE(result);
// }

// TEST(SignedDistTest, Test3) {
//   Plane<double> plane1(0, 0, 0, 1, 0, 0, 0, 1, 0);
//   Plane<double> plane2(1, 1, -1, 1, 1, 1, 0.8, 0.8, 0);

//   double signed_dist11 = plane1.substitute(1, 1, -1);
//   double signed_dist12 = plane1.substitute(1, 1, 1);
//   double signed_dist13 = plane1.substitute(0.8, 0.8, 0);
//   double signed_dist21 = plane2.substitute(0, 0, 0);
//   double signed_dist22 = plane2.substitute(1, 0, 0);
//   double signed_dist23 = plane2.substitute(0, 1, 0);

//   bool result1 = true;
//   bool result2 = true;
//   bool result;

//   if ((signed_dist11 < 0 && signed_dist12 < 0 && signed_dist13 < 0) ||
//       (signed_dist11 > 0 && signed_dist12 > 0 && signed_dist13 > 0)) {
//     result1 = false;
//   }
//   if ((signed_dist21 < 0 && signed_dist22 < 0 && signed_dist23 < 0) ||
//       (signed_dist21 > 0 && signed_dist22 > 0 && signed_dist23 > 0)) {
//     result2 = false;
//   }

//   result = (result1 == true) && (result2 == true);

//   EXPECT_TRUE(result);
// }

// TEST(PointsEqualTest, Test1) {

//   Point<double> point1(1.0, 2.0, 3.0);
//   Point<double> point2(1.0, 2.0, 3.0);

//   bool result = (point1 == point2);
//   EXPECT_TRUE(result);
// }

// TEST(PointsEqualTest, Test2) {
//   Point<double> point1(2.0, 2.0, 3.0);
//   Point<double> point2(1.0, 2.0, 3.0);

//   bool result = (point1 == point2);
//   EXPECT_FALSE(result);
// }

// TEST(PointsEqualTest, Test3) {
//   Point<double> point1(1.0000001, 2.0, 3.0);
//   Point<double> point2(1.0000002, 2.0, 3.0);

//   bool result = (point1 == point2);
//   EXPECT_TRUE(result);
// }

// TEST(SubstitutePointInPlane, Test1) {
//   Plane<double> plane(5, 0, 4, 0, 1, 9, 9, -1, 4);

//   bool result = (plane.substitute(0, 0, 29) == 0);
//   EXPECT_TRUE(result);
// }

// TEST(SubstitutePointInPlane, Test2) {
//   Plane<double> plane(5, 0, 4, 0, 1, 9, 9, -1, 4);

//   bool result = (plane.substitute(0, 0, 30) > 0);
//   EXPECT_TRUE(result);
// }

// TEST(SubstitutePointInPlane, Test3) {
//   Plane<double> plane(5, 0, 4, 0, 1, 9, 9, -1, 4);

//   bool result = (plane.substitute(0, 0, 28) < 0);
//   EXPECT_TRUE(result);
// }

// TEST(SubstitutePointInPlane, Test4) {
//   Plane<double> plane(5, 0, 4, 0, 1, 9, 9, -1, 4);

//   bool result = (plane.substitute(0, 0, -5) >= 0);
//   EXPECT_FALSE(result);
// }

// TEST(PlanesParallel, Test1) {
//   Plane<double> plane1(1, 0, 4, 0, 1, 4, 0, -1, 4);
//   Plane<double> plane2(-1, 0, 3, 0, 1, 3, 1, 0, 3);

//   bool result = planes_are_parallel(plane1, plane2);
//   EXPECT_TRUE(result);
// }

// TEST(PlanesParallel, Test2) {
//   Plane<double> plane1(1, 0, 0, 0, 1, 0, 0, -1, 0);
//   Plane<double> plane2(-1, 0, 0, 0, 1, 0, 1, 0, 0);

//   bool result = planes_are_parallel(plane1, plane2);
//   EXPECT_TRUE(result);
// }

// TEST(PlanesParallel, Test3) {
//   Plane<double> plane1(1, 0, 4, 0, 1, 1, 0, -1, 2);
//   Plane<double> plane2(-1, 0, 3, 0, 1, 3, 1, 0, 3);

//   bool result = planes_are_parallel(plane1, plane2);
//   EXPECT_FALSE(result);
// }

// TEST(PlanesEqualTest, Test1) {
//   Plane<double> plane1(1, 0, 0, 0, 1, 0, 0, -1, 0);
//   Plane<double> plane2(-1, 0, 0, 0, 1, 0, 1, 0, 0);

//   bool result = (plane1 == plane2);
//   EXPECT_TRUE(result);
// }

// TEST(PlanesEqualTest, Test2) {
//   Plane<double> plane1(1, 0, 2, 0, 1, 0, 0, -1, 0);
//   Plane<double> plane2(-1, 0, 0, 0, 1, 0, 1, 0, 0);

//   bool result = (plane1 == plane2);
//   EXPECT_FALSE(result);
// }

// TEST(PlanesEqualTest, Test3) {
//   Plane<double> plane1(1, 0, 4, 0, 1, 4, 0, -1, 4);
//   Plane<double> plane2(-1, 0, 3, 0, 1, 3, 1, 0, 3);

//   bool result = (plane1 == plane2);
//   EXPECT_FALSE(result);
// }
