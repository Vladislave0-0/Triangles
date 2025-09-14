#pragma once
#include <vector>
#include "triangles.hpp"
#include <map>

/// Тип координат (можно менять в одном месте)
using PointTy = double;

/// Запускает окно OpenGL + ImGui для визуализации треугольников.
/// Эта функция блокирует выполнение, пока окно не будет закрыто.
void run_visualizer(std::vector<triangle::Triangle<PointTy>>& input, std::map<size_t, size_t>& intersections);
