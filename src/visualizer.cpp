#include "../include/visualizer.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"

#include <cmath>

using namespace triangle;

// Настройки
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1000;

// Глобальные переменные для настроек
glm::vec3 noIntersectionTriangleColor =
    glm::vec3(27.0f / 255.0f, 94.0f / 255.0f, 215.0f / 255.0f);
glm::vec3 intersectionTriangleColor =
    glm::vec3(215.0f / 255.0f, 27.0f / 255.0f, 27.0f / 255.0f);
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
bool x_grid = false;
bool y_grid = true;
bool z_grid = false;
bool full_grid = false;
bool coordinates = true;

float epsilonShift = 0.01;
float fov = 70.0f;

// Настройки освещения
glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, 1.0f); // Направление света
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // Цвет света
float lightIntensity = 1.0f;

void setupLighting() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Настройка двустороннего освещения
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  GLfloat lightPos[4] = {lightDirection.x, lightDirection.y, lightDirection.z,
                         0.0f};
  GLfloat lightColorData[4] = {lightColor.x * lightIntensity,
                               lightColor.y * lightIntensity,
                               lightColor.z * lightIntensity, 1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColorData);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColorData);

  // Настройки материала
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat shininess[] = {5.0f};

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      cameraPos -= cameraSpeed * cameraUp;
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

  if (full_grid) {
    for (int j = -100; j <= 100; j += 10) {
      for (int i = -100; i <= 100; i += 10) {
        glVertex3f(j, i, -100.0f);
        glVertex3f(j, i, 100.0f);
        glVertex3f(j, -100.0f, i);
        glVertex3f(j, 100.0f, i);
      }
      for (int i = -100; i <= 100; i += 10) {
        glVertex3f(i, j, -100.0f);
        glVertex3f(i, j, 100.0f);
        glVertex3f(-100.0f, j, i);
        glVertex3f(100.0f, j, i);
      }
      for (int i = -100; i <= 100; i += 10) {
        glVertex3f(i, -100.0f, j);
        glVertex3f(i, 100.0f, j);
        glVertex3f(-100.0f, i, j);
        glVertex3f(100.0f, i, j);
      }
    }

    glEnd();
    return;
  }

  if (x_grid) {
    for (int i = -100; i <= 100; ++i) {
      glVertex3f(0.0f, i, -100.0f);
      glVertex3f(0.0f, i, 100.0f);
      glVertex3f(0.0f, -100.0f, i);
      glVertex3f(0.0f, 100.0f, i);
    }
  }

  if (y_grid) {
    for (int i = -100; i <= 100; ++i) {
      glVertex3f(i, 0.0f, -100.0f);
      glVertex3f(i, 0.0f, 100.0f);
      glVertex3f(-100.0f, 0.0f, i);
      glVertex3f(100.0f, 0.0f, i);
    }
  }

  if (z_grid) {
    for (int i = -100; i <= 100; ++i) {
      glVertex3f(i, -100.0f, 0.0f);
      glVertex3f(i, 100.0f, 0.0f);
      glVertex3f(-100.0f, i, 0.0f);
      glVertex3f(100.0f, i, 0.0f);
    }
  }

  glEnd();
}

void drawTriangles(std::vector<Triangle<PointTy>> &input,
                   std::map<size_t, size_t> &intersections, size_t triag_num) {
  glBegin(GL_TRIANGLES);

  for (size_t i = 0; i < triag_num; ++i) {
    if (intersections.find(i / 2) != intersections.end()) {
      glColor3f(intersectionTriangleColor.r, intersectionTriangleColor.g,
                intersectionTriangleColor.b);
    } else {
      glColor3f(noIntersectionTriangleColor.r, noIntersectionTriangleColor.g,
                noIntersectionTriangleColor.b);
    }

    auto a = input[i].get_a();
    auto b = input[i].get_b();
    auto c = input[i].get_c();

    // Вычисление нормали
    glm::vec3 v1 = glm::vec3(b.x - a.x, b.y - a.y, b.z - a.z);
    glm::vec3 v2 = glm::vec3(c.x - a.x, c.y - a.y, c.z - a.z);
    glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

    if (i % 2) {
      normal = -normal;
    }

    glm::vec3 lightDir = glm::normalize(lightDirection);
    float dotProduct = glm::dot(normal, lightDir);

    // Смещение вершин если нормаль направлена к свету
    if (dotProduct >= 0) {
      glm::vec3 offset = epsilonShift * lightDir;
      a.x += offset.x;
      a.y += offset.y;
      a.z += offset.z;
      b.x += offset.x;
      b.y += offset.y;
      b.z += offset.z;
      c.x += offset.x;
      c.y += offset.y;
      c.z += offset.z;
    }

    glNormal3fv(glm::value_ptr(normal));
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glVertex3f(c.x, c.y, c.z);
  }
  glEnd();
}

void drawPauseMenu() {
  ImGui::SetNextWindowPos(
      ImVec2((float)SCR_WIDTH / 2 - 150, (float)SCR_HEIGHT / 2 - 100));
  ImGui::SetNextWindowSize(ImVec2(400, 300));
  ImGui::Begin("Settings", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

  if (ImGui::BeginTabBar("Settings")) {
    if (ImGui::BeginTabItem("Rendering")) {
      ImGui::ColorEdit3("Intersecting", (float *)&noIntersectionTriangleColor);
      ImGui::ColorEdit3("Common", (float *)&intersectionTriangleColor);
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Camera")) {
      ImGui::SliderFloat("Camera Speed", &cameraSpeedBase, 1.0f, 20.0f);
      ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f, "%.1f degrees");
      ImGui::Checkbox("Show Camera's Position", &coordinates);
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Grid")) {
      ImGui::Checkbox("Show X-Grid", &x_grid);
      ImGui::Checkbox("Show Y-Grid", &y_grid);
      ImGui::Checkbox("Show Z-Grid", &z_grid);
      ImGui::Checkbox("Show Full-Grid", &full_grid);
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Light")) {
      ImGui::SliderFloat3("Light Direction", &lightDirection.x, -1.0f, 1.0f);
      ImGui::ColorEdit3("Light Color", &lightColor.x);
      ImGui::SliderFloat("Light Intensity", &lightIntensity, 0.0f, 5.0f);
      ImGui::EndTabItem();
    }
  }

  ImGui::EndTabBar();
  ImGui::End();
}

void drawCameraInfo() {
  ImGui::SetNextWindowPos(ImVec2(10, 10));
  ImGui::SetNextWindowBgAlpha(0.3f);

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

  if (ImGui::Begin("Camera Coordinates", nullptr, flags)) {
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Camera Position");
    ImGui::Separator();
    ImGui::Text("X: %.2f", cameraPos.x);
    ImGui::Text("Y: %.2f", cameraPos.y);
    ImGui::Text("Z: %.2f", cameraPos.z);
  }

  ImGui::End();
}

void run_visualizer(std::vector<triangle::Triangle<PointTy>> &input,
                    std::map<size_t, size_t> &intersections) {
  if (!glfwInit()) {
    std::cerr << "Ошибка: GLFW не инициализировался\n";
    return;
  }

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Scene", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    std::cerr << "Ошибка: не удалось создать окно\n";
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);

  // ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  size_t triag_num = input.size() * 2;
  std::vector<triangle::Triangle<PointTy>> new_input;

  for (size_t i = 0; i < triag_num; ++i) {
    new_input.emplace_back(input[i]);
    new_input.emplace_back(input[i]);
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

    if (coordinates)
      drawCameraInfo();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(
        glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 modelView = view * model;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelView));

    if (x_grid || y_grid || z_grid || full_grid)
      drawGrid();

    setupLighting();

    drawTriangles(new_input, intersections, triag_num);
    // drawNormals(input, triag_num);

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

// void drawNormals(std::vector<Triangle<PointTy>> &input, size_t triag_num) {
//   glColor3f(1.0f, 0.0f, 0.0f);
//   glBegin(GL_LINES);
//   for (size_t i = 0; i < triag_num; ++i) {
//     auto a = input[i].get_a();
//     auto b = input[i].get_b();
//     auto c = input[i].get_c();

//     // Центр треугольника
//     glm::vec3 center((a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f,
//                      (a.z + b.z + c.z) / 3.0f);

//     // Вычисление нормали
//     glm::vec3 v1 = glm::vec3(b.x - a.x, b.y - a.y, b.z - a.z);
//     glm::vec3 v2 = glm::vec3(c.x - a.x, c.y - a.y, c.z - a.z);
//     glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

//     // Инвертируем нормаль для каждого второго треугольника
//     if (i % 2) {
//       normal = -normal;
//     }

//     // Отрисовка нормали
//     glVertex3f(center.x, center.y, center.z);
//     glVertex3f(center.x + normal.x * 0.2f, center.y + normal.y * 0.2f,
//                center.z + normal.z * 0.2f);
//   }
//   glEnd();
// }

// int main() {
//   if (!glfwInit())
//     return -1;

//   GLFWwindow *window =
//       glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Scene", NULL, NULL);
//   if (!window) {
//     glfwTerminate();
//     return -1;
//   }

//   glfwMakeContextCurrent(window);
//   glfwSetCursorPosCallback(window, mouse_callback);
//   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//   glEnable(GL_DEPTH_TEST);
//   glEnable(GL_NORMALIZE);
//   glEnable(GL_LIGHTING);
//   glDisable(GL_CULL_FACE);

//   // ImGui
//   IMGUI_CHECKVERSION();
//   ImGui::CreateContext();
//   ImGuiIO &io = ImGui::GetIO();
//   ImGui::StyleColorsDark();
//   ImGui_ImplGlfw_InitForOpenGL(window, true);
//   ImGui_ImplOpenGL3_Init("#version 130");

//   // Triangles
//   std::vector<Triangle<PointTy>> input;
//   size_t triag_num;
//   std::cin >> triag_num;

//   for (size_t i = 0; i < triag_num; ++i) {
//     PointTy x1, y1, z1, x2, y2, z2, x3, y3, z3;
//     std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

//     input.emplace_back(x1, y1, z1, x2, y2, z2, x3, y3, z3);
//     input.emplace_back(x1, y1, z1, x2, y2, z2, x3, y3, z3);
//   }

//   triag_num *= 2;

//   while (!glfwWindowShouldClose(window)) {
//     float currentFrame = glfwGetTime();
//     deltaTime = currentFrame - lastFrame;
//     lastFrame = currentFrame;

//     processInput(window);

//     ImGui_ImplOpenGL3_NewFrame();
//     ImGui_ImplGlfw_NewFrame();
//     ImGui::NewFrame();

//     if (pause)
//       drawPauseMenu();

//     if (coordinates)
//       drawCameraInfo();

//     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     glm::mat4 projection = glm::perspective(
//         glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,
//         100.0f);
//     glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront,
//     cameraUp); glm::mat4 model = glm::mat4(1.0f); glm::mat4 modelView = view
//     * model;

//     glMatrixMode(GL_PROJECTION);
//     glLoadMatrixf(glm::value_ptr(projection));
//     glMatrixMode(GL_MODELVIEW);
//     glLoadMatrixf(glm::value_ptr(modelView));

//     if (x_grid || y_grid || z_grid || full_grid)
//       drawGrid();

//     setupLighting();

//     drawTriangles(input, triag_num);
//     // drawNormals(input, triag_num);

//     ImGui::Render();
//     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//     glfwSwapBuffers(window);
//     glfwPollEvents();
//   }

//   ImGui_ImplOpenGL3_Shutdown();
//   ImGui_ImplGlfw_Shutdown();
//   ImGui::DestroyContext();

//   glfwTerminate();
// }

// g++ trivial_lighting.cpp ../src/config.cpp \
//     ../imgui/imgui.cpp \
//     ../imgui/imgui_demo.cpp \
//     ../imgui/imgui_draw.cpp \
//     ../imgui/imgui_tables.cpp \
//     ../imgui/imgui_widgets.cpp \
//     ../imgui/backends/imgui_impl_glfw.cpp \
//     ../imgui/backends/imgui_impl_opengl3.cpp \
//     -o exec.x -I../imgui -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -std=c++20
