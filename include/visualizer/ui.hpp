#pragma once

#include "settings.hpp"
#include <GLFW/glfw3.h>

namespace visualizer {
class UISystem {
private:
  UISettings settings; // All UI configuration and state.

public:
  void initialize(GLFWwindow *window);
  void render(bool pause, const glm::vec3 &camera_pos, float fps);

  // Start a new ImGui frame (must be called before any UI rendering).
  void newFrame();

  // Render ImGui draw data to the screen (called after all UI drawing)
  void renderImGui();

  // Clean up ImGui resources and shutdown
  void shutdown();

  UISettings &getSettings() { return settings; }

  // Individual UI component rendering methods
  void drawPauseMenu();
  void drawCameraInfo(const glm::vec3 &camera_pos);
  void drawFPS(float fps);
};
} // namespace visualizer
