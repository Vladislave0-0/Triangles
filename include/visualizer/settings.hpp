#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

namespace visualizer {
struct GridSettings {
  bool x_grid = false;
  bool y_grid = true;
  bool z_grid = false;
  bool full_grid = false;
  float range = 100.0f;
  float full_step = 10.0f;
  float non_full_step = 1.0f;
};

struct UISettings {
  glm::vec3 no_intersection_color;
  glm::vec3 intersection_color;
  float drawing_range = 100.0f;

  float camera_speed_base = 20.0f;
  float fov = 70.0f;
  bool show_coordinates = true;

  bool show_fps = true;
  ImVec4 fps_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

  glm::vec3 light_direction = glm::vec3(0.0f, 0.0f, 1.0f);
  glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
  float light_intensity = 1.0f;

  GridSettings grid_settings;
};
} // namespace visualizer
