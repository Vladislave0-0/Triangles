#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/triangles.hpp"

using PointTy = double;
using namespace triangle;

// Настройки
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 triangleColor = glm::vec3(0.0f, 1.0f, 0.0f); // Исходный цвет
bool colorChanged = false;
float buttonSize = 50.0f;

// Камера
glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.5f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Тайминг
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Управление
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool pause = false;

void processInput(GLFWwindow *window) {
  static bool escPressedLastFrame = false;
  float cameraSpeed = 10.0f * deltaTime;

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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cameraPos -=
          glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cameraPos +=
          glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
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

  float sensitivity = 0.4f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  pitch = glm::clamp(pitch, -89.0f, 89.0f);

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}

void drawGrid() {
  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_LINES);
  for (int i = -100; i <= 100; ++i) {
    glVertex3f(i, 0.0f, -100.0f);
    glVertex3f(i, 0.0f, 100.0f);
    glVertex3f(-100.0f, 0.0f, i);
    glVertex3f(100.0f, 0.0f, i);
  }
  glEnd();
}

void drawTriangles(std::vector<Triangle<PointTy>> &input, size_t triag_num) {
  glColor3f(triangleColor.r, triangleColor.g, triangleColor.b);
  glBegin(GL_TRIANGLES);
  for (size_t i = 0; i < triag_num; ++i) {
    glVertex3f(input[i].get_a().x, input[i].get_a().y, input[i].get_a().z);
    glVertex3f(input[i].get_b().x, input[i].get_b().y, input[i].get_b().z);
    glVertex3f(input[i].get_c().x, input[i].get_c().y, input[i].get_c().z);
  }
  glEnd();
}

bool isColorButtonClicked(float mouseX, float mouseY, glm::vec2 position) {
  return (mouseX > position.x && mouseX < position.x + buttonSize &&
          mouseY > position.y && mouseY < position.y + buttonSize);
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (pause && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Позиции кнопок
    glm::vec2 redBtnPos(SCR_WIDTH / 2 - 100, SCR_HEIGHT / 2 - 50);
    glm::vec2 greenBtnPos(SCR_WIDTH / 2 - 25, SCR_HEIGHT / 2 - 50);
    glm::vec2 blueBtnPos(SCR_WIDTH / 2 + 50, SCR_HEIGHT / 2 - 50);

    if (isColorButtonClicked(xpos, ypos, redBtnPos)) {
      triangleColor = glm::vec3(1.0f, 0.0f, 0.0f);
    } else if (isColorButtonClicked(xpos, ypos, greenBtnPos)) {
      triangleColor = glm::vec3(0.0f, 1.0f, 0.0f);
    } else if (isColorButtonClicked(xpos, ypos, blueBtnPos)) {
      triangleColor = glm::vec3(0.0f, 0.4f, 1.0f);
    }
  }
}

void drawMenu() {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, SCR_WIDTH, SCR_HEIGHT, 0, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);

  // Фон меню
  float menuWidth = 300.0f;
  float menuHeight = 200.0f;
  float x = (SCR_WIDTH - menuWidth) / 2;
  float y = (SCR_HEIGHT - menuHeight) / 2;

  glColor3f(0.2f, 0.3f, 0.3f);
  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x + menuWidth, y);
  glVertex2f(x + menuWidth, y + menuHeight);
  glVertex2f(x, y + menuHeight);
  glEnd();

  // Кнопки выбора цвета
  glm::vec2 redBtnPos(SCR_WIDTH / 2 - 100, SCR_HEIGHT / 2 - 50);
  glm::vec2 greenBtnPos(SCR_WIDTH / 2 - 25, SCR_HEIGHT / 2 - 50);
  glm::vec2 blueBtnPos(SCR_WIDTH / 2 + 50, SCR_HEIGHT / 2 - 50);

  // Красная кнопка
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_QUADS);
  glVertex2f(redBtnPos.x, redBtnPos.y);
  glVertex2f(redBtnPos.x + buttonSize, redBtnPos.y);
  glVertex2f(redBtnPos.x + buttonSize, redBtnPos.y + buttonSize);
  glVertex2f(redBtnPos.x, redBtnPos.y + buttonSize);
  glEnd();

  // Зеленая кнопка
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_QUADS);
  glVertex2f(greenBtnPos.x, greenBtnPos.y);
  glVertex2f(greenBtnPos.x + buttonSize, greenBtnPos.y);
  glVertex2f(greenBtnPos.x + buttonSize, greenBtnPos.y + buttonSize);
  glVertex2f(greenBtnPos.x, greenBtnPos.y + buttonSize);
  glEnd();

  // Синяя кнопка
  glColor3f(0.0f, 0.4f, 1.0f);
  glBegin(GL_QUADS);
  glVertex2f(blueBtnPos.x, blueBtnPos.y);
  glVertex2f(blueBtnPos.x + buttonSize, blueBtnPos.y);
  glVertex2f(blueBtnPos.x + buttonSize, blueBtnPos.y + buttonSize);
  glVertex2f(blueBtnPos.x, blueBtnPos.y + buttonSize);
  glEnd();

  // Восстановление состояния
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

int main() {
  if (!glfwInit())
    return -1;

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Scene", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);

  // Triangles
  std::vector<Triangle<PointTy>> input;
  size_t triag_num;
  std::cin >> triag_num;

  for (size_t i = 0; i < triag_num; ++i) {
    PointTy x1, y1, z1, x2, y2, z2, x3, y3, z3;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
    input.emplace_back(x1, y1, z1, x2, y2, z2, x3, y3, z3);
  }

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));

    drawGrid();
    drawTriangles(input, triag_num);

    if (pause)
      drawMenu();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}

// g++ testing_pause_menu_modifications.cpp ../src/config.cpp -o exec.x -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -std=c++20
