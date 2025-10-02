#include "visualizer/shader.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace visualizer {

ShaderSystem::~ShaderSystem() { cleanup(); }

bool ShaderSystem::initialize() {
  return (shaderProgram = createShaderProgram(), shaderProgram != 0);
}

void ShaderSystem::setupTriangleBuffers(
    const std::vector<triangle::Triangle<PointTy>> &regular_triangles,
    const std::vector<triangle::Triangle<PointTy>> &intersecting_triangles,
    const glm::vec3 &regular_color, const glm::vec3 &intersect_color) {
  setupBuffer(regular_triangles, regularVAO, regularVBO, regularVertexCount,
              regular_color);
  setupBuffer(intersecting_triangles, intersectVAO, intersectVBO,
              intersectVertexCount, intersect_color);
}

// Render all triangles (regular first, then intersecting).
void ShaderSystem::render() {
  glUseProgram(shaderProgram);

  glBindVertexArray(regularVAO);
  glDrawArrays(GL_TRIANGLES, 0, regularVertexCount);

  glBindVertexArray(intersectVAO);
  glDrawArrays(GL_TRIANGLES, 0, intersectVertexCount);

  glBindVertexArray(0);
}

void ShaderSystem::cleanup() {
  if (regularVAO) {
    glDeleteVertexArrays(1, &regularVAO);
    regularVAO = 0;
  }
  if (regularVBO) {
    glDeleteBuffers(1, &regularVBO);
    regularVBO = 0;
  }
  if (intersectVAO) {
    glDeleteVertexArrays(1, &intersectVAO);
    intersectVAO = 0;
  }
  if (intersectVBO) {
    glDeleteBuffers(1, &intersectVBO);
    intersectVBO = 0;
  }
  if (shaderProgram) {
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
  }
}

void ShaderSystem::setMatrices(const glm::mat4 &model, const glm::mat4 &view,
                               const glm::mat4 &projection) {
  glUseProgram(shaderProgram);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                     glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));
}

void ShaderSystem::setLighting(const glm::vec3 &lightDir,
                               const glm::vec3 &lightColor,
                               float lightIntensity, const glm::vec3 &viewPos) {
  glUseProgram(shaderProgram);
  glUniform3fv(glGetUniformLocation(shaderProgram, "lightDir"), 1,
               glm::value_ptr(lightDir));
  glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1,
               glm::value_ptr(lightColor));
  glUniform1f(glGetUniformLocation(shaderProgram, "lightIntensity"),
              lightIntensity);
  glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1,
               glm::value_ptr(viewPos));
}

GLuint ShaderSystem::compileShader(const char *source, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    return 0;
  }

  return shader;
}

GLuint ShaderSystem::createShaderProgram() {

  // Vertex shader: transforms vertices and calculates lighting vectors.
  const char *vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec3 aColor;
            
            out vec3 FragPos;
            out vec3 Normal;
            out vec3 Color;
            out vec3 ViewDir;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            uniform vec3 viewPos;
            
            void main() {
                FragPos = vec3(model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(model))) * aNormal;
                Color = aColor;
                
                ViewDir = viewPos - FragPos;
                
                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
        )";

  // Fragment shader: calculates Phong lighting and outputs final color.
  const char *fragmentShaderSource = R"(
    #version 330 core
    in vec3 FragPos;
    in vec3 Normal;
    in vec3 Color;
    in vec3 ViewDir;

    out vec4 FragColor;

    uniform vec3 lightDir;
    uniform vec3 lightColor;
    uniform float lightIntensity;
    uniform vec3 viewPos;

    void main() {
        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(ViewDir);
        
        float facing = dot(norm, viewDir);
        
        if (facing < 0.0) {
            norm = -norm;
        }
        
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
        
        vec3 lightDirection = normalize(-lightDir);
        float diff = max(dot(norm, lightDirection), 0.0);
        vec3 diffuse = diff * lightColor * lightIntensity;
        
        float specularStrength = 0.5;
        vec3 reflectDir = reflect(-lightDirection, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        
        vec3 result = (ambient + diffuse + specular) * Color;
        FragColor = vec4(result, 1.0);
    })";

  // Compile both shaders.
  GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
  if (vertexShader == 0)
    return 0;

  GLuint fragmentShader =
      compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  if (fragmentShader == 0) {
    glDeleteShader(vertexShader);
    return 0;
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

// Set up vertex buffer for a specific set of triangles.
void ShaderSystem::setupBuffer(
    const std::vector<triangle::Triangle<PointTy>> &triangles, GLuint &VAO,
    GLuint &VBO, int &vertexCount, const glm::vec3 &color) {
  std::vector<Vertex> vertices;

  // Convert each triangle to vertex data with calculated normals.
  for (const auto &triangle : triangles) {
    auto a = triangle.get_a();
    auto b = triangle.get_b();
    auto c = triangle.get_c();

    // Calculate triangle normal using cross product.
    glm::vec3 v1 = glm::vec3(b.x - a.x, b.y - a.y, b.z - a.z);
    glm::vec3 v2 = glm::vec3(c.x - a.x, c.y - a.y, c.z - a.z);
    glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

    // Add three vertices for the triangle (same normal for all vertices).
    vertices.push_back({{a.x, a.y, a.z}, normal, color});
    vertices.push_back({{b.x, b.y, b.z}, normal, color});
    vertices.push_back({{c.x, c.y, c.z}, normal, color});
  }

  vertexCount = vertices.size();

  // Create and configure OpenGL buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  // Set up vertex attribute pointers for interleaved data.
  // Position attribute (location = 0).
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Normal attribute (location = 1).
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Color attribute (location = 2)
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}
} // namespace visualizer
