#include "../include/triangles.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Добавляем заголовки ImGui
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"

using PointTy = double;
using namespace triangle;

// Настройки
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Глобальные переменные для настроек
glm::vec3 triangleColor = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeedBase = 10.0f;

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
bool greed = false;

void processInput(GLFWwindow *window) {
  static bool escPressedLastFrame = false;
  float cameraSpeed = cameraSpeedBase * deltaTime;

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

void drawPauseMenu() {
  ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH / 2 - 150, SCR_HEIGHT / 2 - 100));
  ImGui::SetNextWindowSize(ImVec2(400, 300));
  ImGui::Begin("Settings", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

  // Выбор цвета + палитра
  ImGui::ColorEdit3("Triangle Color", (float *)&triangleColor);

  // Скорость для камеры
  ImGui::SliderFloat("Camera Speed", &cameraSpeedBase, 1.0f, 20.0f);

  // Показывать сетку
  ImGui::Checkbox("Show Greed", &greed);

  ImGui::End();
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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);

  // ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (pause)
      drawPauseMenu();

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

    if (greed)
      drawGrid();

    drawTriangles(input, triag_num);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
}

// g++ testing_better_pause_menu.cpp ../src/config.cpp \
//     ../imgui/imgui.cpp \
//     ../imgui/imgui_demo.cpp \
//     ../imgui/imgui_draw.cpp \
//     ../imgui/imgui_tables.cpp \
//     ../imgui/imgui_widgets.cpp \
//     ../imgui/backends/imgui_impl_glfw.cpp \
//     ../imgui/backends/imgui_impl_opengl3.cpp \
//     -o exec.x -I../imgui -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -std=c++20
