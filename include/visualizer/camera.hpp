#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace visualizer {
class CameraSystem {
private:
  glm::vec3 position; // Camera position in world space
  glm::vec3 front;    // Camera direction vector
  glm::vec3 up;       // Up vector for camera orientation
  float yaw, pitch;   // Euler angles for camera rotation
  float speed;        // Camera movement speed
  float sensitivity;  // Mouse sensitivity for camera rotation

  float lastX, lastY; // Last mouse position for delta calculation
  bool firstMouse;    // Flag for initial mouse movement handling
  bool &pause;        // Reference to pause state

public:
  CameraSystem(bool &pause_ref);
  void initialize(unsigned int screen_width, unsigned int screen_height);

  // Handle keyboard input for camera movement.
  void processInput(GLFWwindow *window, float deltaTime);

  // Handle mouse movement for camera rotation.
  void mouseCallback(GLFWwindow *window, double xpos, double ypos);

  // Getters for camera properties.
  glm::mat4 getViewMatrix() const;
  glm::vec3 getPosition() const { return position; }
  glm::vec3 getFront() const { return front; }
  glm::vec3 getUp() const { return up; }
  float getYaw() const { return yaw; }
  float getPitch() const { return pitch; }

  // Setters for camera properties.
  void setPosition(const glm::vec3 &pos) { position = pos; }
  void setFront(const glm::vec3 &f) { front = f; }
  void setYaw(float y) { yaw = y; }
  void setPitch(float p) { pitch = p; }

  // Set camera movement speed.
  void setSpeed(float newSpeed) { speed = newSpeed; }

private:
  // Update camera vectors based on yaw and pitch angles.
  void updateCameraVectors();
};
} // namespace visualizer
