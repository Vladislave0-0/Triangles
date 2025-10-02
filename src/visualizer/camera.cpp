#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "visualizer/camera.hpp"

namespace visualizer {

CameraSystem::CameraSystem(bool &pause_ref)
    : position(glm::vec3(0.0f, 3.0f, 3.0f)),
      front(glm::vec3(0.0f, -0.5f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
      yaw(-90.0f), pitch(0.0f), speed(20.0f), sensitivity(0.4f), lastX(0.0f),
      lastY(0.0f), firstMouse(true), pause(pause_ref) {}

void CameraSystem::initialize(unsigned int screen_width,
                              unsigned int screen_height) {
  lastX = screen_width / 2.0f;
  lastY = screen_height / 2.0f;
  updateCameraVectors();
}

void CameraSystem::processInput(GLFWwindow *window, float deltaTime) {
  static bool escPressedLastFrame = false;

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  bool currentEscState = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
  if (currentEscState && !escPressedLastFrame) {
    pause = !pause;

    glfwSetInputMode(window, GLFW_CURSOR,
                     pause ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    if (!pause)
      firstMouse = true;
  }
  escPressedLastFrame = currentEscState;

  if (!pause) {
    float cameraSpeed = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      position += cameraSpeed * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      position -= cameraSpeed * front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      position += cameraSpeed * up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      position -= cameraSpeed * up;
  }
}

void CameraSystem::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (pause)
    return;

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  pitch = glm::clamp(pitch, -89.0f, 89.0f);

  updateCameraVectors();
}

glm::mat4 CameraSystem::getViewMatrix() const {
  return glm::lookAt(position, position + front, up);
}

void CameraSystem::updateCameraVectors() {
  glm::vec3 newFront;
  newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  newFront.y = sin(glm::radians(pitch));
  newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(newFront);
}
} // namespace visualizer
