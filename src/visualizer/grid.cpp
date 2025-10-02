#include "visualizer/grid.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace visualizer {

// Vertex shader for grid rendering - transforms grid vertices.
const char *gridVertexShader = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * vec4(aPos, 1.0);
    }
)";

// Fragment shader for grid rendering - outputs grid line color.
const char *gridFragmentShader = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 gridColor;

    void main() {
        FragColor = vec4(gridColor, 1.0);
    }
)";

void GridSystem::initialize() {
  setupGridShader();
  createGridData();
}

void GridSystem::setupGridShader() {
  // Compile vertex shader.
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &gridVertexShader, NULL);
  glCompileShader(vertexShader);

  // Compile fragment shader.
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &gridFragmentShader, NULL);
  glCompileShader(fragmentShader);

  // Create shader program and link shaders.
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Clean up shader objects (they're linked into the program).
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void GridSystem::createGridData() {
  std::vector<glm::vec3> vertices;
  float range = settings.range;
  float step = settings.non_full_step;

  // Generate geometry based on grid type.
  if (settings.full_grid) {
    float step = settings.full_step;

    // Lines parallel to X-axis (varying Y and Z).
    for (float y = -range; y <= range; y += step) {
      for (float z = -range; z <= range; z += step) {
        vertices.push_back(glm::vec3(-range, y, z));
        vertices.push_back(glm::vec3(range, y, z));
      }
    }

    // Lines parallel to Y-axis (varying X and Z).
    for (float x = -range; x <= range; x += step) {
      for (float z = -range; z <= range; z += step) {
        vertices.push_back(glm::vec3(x, -range, z));
        vertices.push_back(glm::vec3(x, range, z));
      }
    }

    // Lines parallel to Z-axis (varying X and Y).
    for (float x = -range; x <= range; x += step) {
      for (float y = -range; y <= range; y += step) {
        vertices.push_back(glm::vec3(x, y, -range));
        vertices.push_back(glm::vec3(x, y, range));
      }
    }
  } else {
    // X-plane grid (plane where X = 0).
    if (settings.x_grid) {
      for (float i = -range; i <= range; i += step) {
        vertices.push_back(glm::vec3(0.0f, i, -range));
        vertices.push_back(glm::vec3(0.0f, i, range));
        vertices.push_back(glm::vec3(0.0f, -range, i));
        vertices.push_back(glm::vec3(0.0f, range, i));
      }
    }

    // Y-plane grid (plane where Y = 0) - default plane.
    if (settings.y_grid) {
      for (float i = -range; i <= range; i += step) {
        vertices.push_back(glm::vec3(-range, 0.0f, i));
        vertices.push_back(glm::vec3(range, 0.0f, i));
        vertices.push_back(glm::vec3(i, 0.0f, -range));
        vertices.push_back(glm::vec3(i, 0.0f, range));
      }
    }

    // Z-plane grid (plane where Z = 0).
    if (settings.z_grid) {
      for (float i = -range; i <= range; i += step) {
        vertices.push_back(glm::vec3(i, -range, 0.0f));
        vertices.push_back(glm::vec3(i, range, 0.0f));
        vertices.push_back(glm::vec3(-range, i, 0.0f));
        vertices.push_back(glm::vec3(range, i, 0.0f));
      }
    }
  }

  // Create VAO and VBO.
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

void GridSystem::render(const glm::mat4 &view, const glm::mat4 &projection,
                        const glm::vec3 &cameraPos, float fov,
                        float drawing_range, unsigned int screen_width,
                        unsigned int screen_height) {
  // Early exit if no grid types are enabled.
  if (!(settings.x_grid || settings.y_grid || settings.z_grid ||
        settings.full_grid)) {
    return;
  }

  // Use grid shader program.
  glUseProgram(shaderProgram);

  // Pass transformation matrices to shader.
  GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
  GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
  GLint colorLoc = glGetUniformLocation(shaderProgram, "gridColor");

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
  glUniform3f(colorLoc, 0.7f, 0.7f, 0.7f); // Light gray color for grid lines

  // Render grid lines.
  glBindVertexArray(VAO);
  glDrawArrays(GL_LINES, 0, settings.range * 100);
  glBindVertexArray(0);
}

} // namespace visualizer
