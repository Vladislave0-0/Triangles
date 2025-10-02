#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../triangles.hpp"

using PointTy = double;

namespace visualizer {
struct Vertex {
  glm::vec3 position; // 3D position in world space
  glm::vec3 normal;   // Surface normal for lighting calculations
  glm::vec3 color;    // RGB color of the vertex
};

class ShaderSystem {
private:
  GLuint shaderProgram = 0;     // Main shader program for triangle rendering
  GLuint regularVAO = 0;        // VAO for non-intersecting triangles
  GLuint regularVBO = 0;        // VBO for non-intersecting triangles
  GLuint intersectVAO = 0;      // VAO for intersecting triangles
  GLuint intersectVBO = 0;      // VBO for intersecting triangles
  int regularVertexCount = 0;   // Number of vertices in non-intersecting buffer
  int intersectVertexCount = 0; // Number of vertices in intersecting buffer

public:
  ShaderSystem() = default;
  ~ShaderSystem();

  bool initialize();
  void setupTriangleBuffers(
      const std::vector<triangle::Triangle<PointTy>> &regular_triangles,
      const std::vector<triangle::Triangle<PointTy>> &intersecting_triangles,
      const glm::vec3 &regular_color, const glm::vec3 &intersect_color);

  // Render all triangles.
  void render();
  void cleanup();

  // Set transformation matrices for vertex shader.
  void setMatrices(const glm::mat4 &model, const glm::mat4 &view,
                   const glm::mat4 &projection);

  // Get the shader program ID for external use.
  void setLighting(const glm::vec3 &lightDir, const glm::vec3 &lightColor,
                   float lightIntensity, const glm::vec3 &viewPos);

  // Get the shader program ID for external use.
  GLuint getProgram() const { return shaderProgram; }

private:
  // Compile individual shader from source code.
  GLuint compileShader(const char *source, GLenum type);

  // Create and link complete shader program.
  GLuint createShaderProgram();

  void setupBuffer(const std::vector<triangle::Triangle<PointTy>> &triangles,
                   GLuint &VAO, GLuint &VBO, int &vertexCount,
                   const glm::vec3 &color);
};
} // namespace visualizer
