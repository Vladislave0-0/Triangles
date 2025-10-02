#include "visualizer/flag.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace visualizer {

// Vertex shader for flag rendering - transforms vertices and passes color.
const char *flagVertexShader = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;

    out vec3 Color;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        Color = aColor;
    }
)";

// Fragment shader for flag rendering - outputs final color.
const char *flagFragmentShader = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 Color;

    void main() {
        FragColor = vec4(Color, 1.0);
    }
)";

void FlagSystem::initialize() {
  setupFlagShader();
  createFlagData();
}

void FlagSystem::setupFlagShader() {
  // Compile vertex shader.
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &flagVertexShader, NULL);
  glCompileShader(vertexShader);

  // Check vertex shader compilation errors.
  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Flag Vertex shader compilation failed: " << infoLog
              << std::endl;
  }

  // Compile fragment shader.
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &flagFragmentShader, NULL);
  glCompileShader(fragmentShader);

  // Check fragment shader compilation errors.
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Flag Fragment shader compilation failed: " << infoLog
              << std::endl;
  }

  // Create shader program and link shaders.
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check shader program linking errors.
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "Flag Shader program linking failed: " << infoLog << std::endl;
  }

  // Clean up shader objects.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void FlagSystem::addStripe(std::vector<float> &vertices,
                           const glm::vec2 &topLeft,
                           const glm::vec2 &bottomRight,
                           const glm::vec3 &color) {
  float left = topLeft.x;
  float top = topLeft.y;
  float right = bottomRight.x;
  float bottom = bottomRight.y;

  float r = color.r;
  float g = color.g;
  float b = color.b;

  vertices.insert(vertices.end(),
                  {
                      left, top, 0.0f, r, g, b,    // top-left
                      right, top, 0.0f, r, g, b,   // top-right
                      left, bottom, 0.0f, r, g, b, // bottom-left
                  });

  vertices.insert(vertices.end(),
                  {
                      right, top, 0.0f, r, g, b,    // top-right
                      right, bottom, 0.0f, r, g, b, // bottom-right
                      left, bottom, 0.0f, r, g, b,  // bottom-left
                  });
}

// Create flag geometry with three colored stripes (Russian flag colors)
void FlagSystem::createFlagData() {
  std::vector<float> vertices; // Interleaved data: position (xyz) + color (rgb)

  float flagWidth = 3.0f;                 // Total flag width
  float flagHeight = 2.0f;                // Total flag height
  float stripeHeight = flagHeight / 3.0f; // Height of each colored stripe

  float cur_flag_width = flagWidth / 2.0f;   // Half-width for centering
  float cur_flag_height = flagHeight / 2.0f; // Half-height for centering

  const glm::vec3 whiteColor(1.0f, 1.0f, 1.0f);
  const glm::vec3 blueColor(0.0f, 0.0f, 1.0f);
  const glm::vec3 redColor(1.0f, 0.0f, 0.0f);

  // White stripe.
  addStripe(vertices, glm::vec2(-cur_flag_width, cur_flag_height),
            glm::vec2(cur_flag_width, cur_flag_height - stripeHeight),
            whiteColor);

  // Blue stripe.
  addStripe(
      vertices, glm::vec2(-cur_flag_width, cur_flag_height - stripeHeight),
      glm::vec2(cur_flag_width, cur_flag_height - 2 * stripeHeight), blueColor);

  // Red stripe.
  addStripe(vertices,
            glm::vec2(-cur_flag_width, cur_flag_height - 2 * stripeHeight),
            glm::vec2(cur_flag_width, -cur_flag_height), redColor);

  // Create VAO and VBO.
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Set up VAO and VBO.
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  // Set up vertex attribute for positions (location 0).
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Set up vertex attribute for colors (location 1).
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void FlagSystem::render(const glm::vec3 &camera_pos,
                        const glm::vec3 &camera_front, float yaw, float pitch,
                        const glm::mat4 &view, const glm::mat4 &projection,
                        unsigned int screen_width, unsigned int screen_height,
                        float fov, float drawing_range) {

  if (!show_flag)
    return;

  // Use flag shader program.
  glUseProgram(shaderProgram);

  // Fixed distance in front of camera.
  glm::vec3 position = camera_pos + camera_front * distance;

  glm::mat4 model = glm::mat4(1.0f);

  // Position flag in world space.
  model = glm::translate(model, position);

  // Rotate flag to face camera (simplified billboarding). Uses camera yaw and
  // pitch to orient the flag.
  model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-pitch), glm::vec3(1.0f, 0.0f, 0.0f));

  // Pass transformation matrices to shader.
  GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
  GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
  GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 18); // 6 vertices per stripe * 3 stripes
  glBindVertexArray(0);
}

} // namespace visualizer
