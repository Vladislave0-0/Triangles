#pragma once

#include <GL/glew.h>

#include "settings.hpp"

namespace visualizer {
class GridSystem {
private:
  GridSettings settings; // Grid configuration settings
  GLuint shaderProgram;  // Shader program for grid rendering
  GLuint VAO, VBO;       // Vertex Array and Buffer Objects

  // Compile and link grid shader program.
  void setupGridShader();

  // Generate grid geometry based on current settings.
  void createGridData();

public:
  void initialize();
  void render(const glm::mat4 &view, const glm::mat4 &projection,
              const glm::vec3 &cameraPos, float fov, float drawing_range,
              unsigned int screen_width, unsigned int screen_height);

  // Update grid settings.
  void setSettings(const GridSettings &newSettings) { settings = newSettings; }

  // Get current grid settings.
  GridSettings &getSettings() { return settings; }
};
} // namespace visualizer
