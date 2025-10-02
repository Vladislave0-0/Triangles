#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace visualizer {
class FlagSystem {
private:
  bool show_flag = false; // Flag visibility state
  float distance = 5.0f;  // Distance from camera to flag
  GLuint shaderProgram;   // Shader program for flag rendering
  GLuint VAO, VBO;        // Vertex Array and Buffer Objects

  void setupFlagShader();
  void createFlagData();

  // Add a colored stripe to the flag.
  void addStripe(std::vector<float> &vertices, const glm::vec2 &topLeft,
                 const glm::vec2 &bottomRight, const glm::vec3 &color);

public:
  void initialize();
  void render(const glm::vec3 &camera_pos, const glm::vec3 &camera_front,
              float yaw, float pitch, const glm::mat4 &view,
              const glm::mat4 &projection, unsigned int screen_width,
              unsigned int screen_height, float fov, float drawing_range);

  // Toggle flag visibility.
  void toggle() { show_flag = !show_flag; }

  // Check if flag is currently visible.
  bool isVisible() const { return show_flag; }
};
} // namespace visualizer
