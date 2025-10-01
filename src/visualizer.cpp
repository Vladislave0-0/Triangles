#include "../include/visualizer.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"

using namespace triangle;

// Window size.
unsigned int screen_width = 0;
unsigned int screen_height = 0;

// Variables for different colors of triangles depending on whether they
// intersect or not.
glm::vec3 no_intersection_color =
    glm::vec3(27.0f / 255.0f, 94.0f / 255.0f, 215.0f / 255.0f);
glm::vec3 intersection_color =
    glm::vec3(215.0f / 255.0f, 27.0f / 255.0f, 27.0f / 255.0f);

// Camera.
float yaw = -90.0f;
float pitch = 0.0f;
glm::vec3 camera_pos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, -0.5f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
float camera_speed_base = 20.0f;

// Timing.
float delta_time = 0.0f;
float last_frame = 0.0f;

// FPS.
bool show_FPS = true;
ImVec4 fps_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

// Control.
float last_x = screen_width / 2.0f;
float last_y = screen_height / 2.0f;
float epsilon_shift = 0.01;
bool first_mouse = true;
bool pause = false;
bool coordinates = true;

// View.
float fov = 70.0f;
float drawing_range = 100.0f;

// Grid.
bool x_grid = false;
bool y_grid = true;
bool z_grid = false;
bool full_grid = false;
float grid_range = 100.0f;
float full_grid_step = 10.0f;
float non_full_grid_step = 1.0f;

// Light.
glm::vec3 light_direction = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
float light_intensity = 1.0f;

// Flag
bool show_flag = false;
float flag_distance = 5.0f;
float flag_size = 2.0f;

static void setupLighting() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Setting up two-way lighting.
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  GLfloat lightPos[4] = {light_direction.x, light_direction.y,
                         light_direction.z, 0.0f};
  GLfloat lightColorData[4] = {light_color.x * light_intensity,
                               light_color.y * light_intensity,
                               light_color.z * light_intensity, 1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColorData);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColorData);

  // Material Settings.
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat shininess[] = {5.0f};

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

static void processInput(GLFWwindow *window) {
  static bool escPressedLastFrame = false;
  float cameraSpeed = camera_speed_base * delta_time;

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  bool currentEscState = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
  if (currentEscState && !escPressedLastFrame) {
    pause = !pause;
    glfwSetInputMode(window, GLFW_CURSOR,
                     pause ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    if (!pause)
      first_mouse = true;
  }
  escPressedLastFrame = currentEscState;

  if (!pause) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera_pos += cameraSpeed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera_pos -= cameraSpeed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera_pos -=
          glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera_pos +=
          glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      camera_pos += cameraSpeed * camera_up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      camera_pos -= cameraSpeed * camera_up;
  }

  static bool flagToggle = false;
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !flagToggle) {
    show_flag = !show_flag;
    flagToggle = true;
  } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
    flagToggle = false;
  }
}

static void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (pause)
    return;

  if (first_mouse) {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }

  float xoffset = xpos - last_x;
  float yoffset = last_y - ypos;
  last_x = xpos;
  last_y = ypos;

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
  camera_front = glm::normalize(front);
}

static void drawGrid() {
  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_LINES);

  int grid_range_int = static_cast<int>(grid_range);
  int full_grid_step_int = static_cast<int>(full_grid_step);
  int non_full_grid_step_int = static_cast<int>(non_full_grid_step);

  // Drawing a spatial grid.
  if (full_grid) {
    for (int j = -grid_range_int; j <= grid_range_int;
         j += full_grid_step_int) {
      for (int i = -grid_range_int; i <= grid_range_int;
           i += full_grid_step_int) {
        glVertex3f(j, i, -grid_range);
        glVertex3f(j, i, grid_range);
        glVertex3f(j, -grid_range, i);
        glVertex3f(j, grid_range, i);
      }
      for (int i = -grid_range_int; i <= grid_range_int;
           i += full_grid_step_int) {
        glVertex3f(i, j, -grid_range);
        glVertex3f(i, j, grid_range);
        glVertex3f(-grid_range, j, i);
        glVertex3f(grid_range, j, i);
      }
      for (int i = -grid_range_int; i <= grid_range_int;
           i += full_grid_step_int) {
        glVertex3f(i, -grid_range, j);
        glVertex3f(i, grid_range, j);
        glVertex3f(-grid_range, i, j);
        glVertex3f(grid_range, i, j);
      }
    }

    glEnd();
    return;
  }

  // Drawing a grid in the x = 0, y = 0, z = 0 planes separately.
  if (x_grid) {
    for (int i = -grid_range_int; i <= grid_range_int;
         i += non_full_grid_step_int) {
      glVertex3f(0.0f, i, -grid_range);
      glVertex3f(0.0f, i, grid_range);
      glVertex3f(0.0f, -grid_range, i);
      glVertex3f(0.0f, grid_range, i);
    }
  }

  if (y_grid) {
    for (int i = -grid_range_int; i <= grid_range_int;
         i += non_full_grid_step_int) {
      glVertex3f(i, 0.0f, -grid_range);
      glVertex3f(i, 0.0f, grid_range);
      glVertex3f(-grid_range, 0.0f, i);
      glVertex3f(grid_range, 0.0f, i);
    }
  }

  if (z_grid) {
    for (int i = -grid_range_int; i <= grid_range_int;
         i += non_full_grid_step_int) {
      glVertex3f(i, -grid_range, 0.0f);
      glVertex3f(i, grid_range, 0.0f);
      glVertex3f(-grid_range, i, 0.0f);
      glVertex3f(grid_range, i, 0.0f);
    }
  }

  glEnd();
}

static void drawTriangles(std::vector<Triangle<PointTy>> &input,
                          std::map<size_t, size_t> &intersections,
                          size_t triag_num) {
  glBegin(GL_TRIANGLES);

  // Change the color depending on whether the triangles intersect or not.
  for (size_t i = 0; i < triag_num; ++i) {
    if (intersections.find(i / 2) != intersections.end()) {
      glColor3f(intersection_color.r, intersection_color.g,
                intersection_color.b);
    } else {
      glColor3f(no_intersection_color.r, no_intersection_color.g,
                no_intersection_color.b);
    }

    auto a = input[i].get_a();
    auto b = input[i].get_b();
    auto c = input[i].get_c();

    // Calculating the normal.
    glm::vec3 v1 = glm::vec3(b.x - a.x, b.y - a.y, b.z - a.z);
    glm::vec3 v2 = glm::vec3(c.x - a.x, c.y - a.y, c.z - a.z);
    glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

    // For each even triangle, the normal changes its direction because the
    // circumambulation of the triangle changes (each even triangle is an exact
    // copy of the odd triangle).
    if (i % 2)
      normal = -normal;

    glm::vec3 lightDir = glm::normalize(light_direction);
    float dotProduct = glm::dot(normal, lightDir);

    // We shift all the normals that are opposite to the light source (that is,
    // these are the normal triangles that should be illuminated) by a small
    // amount so that there is no flickering effect when working with OpenGL.
    if (dotProduct >= 0) {
      glm::vec3 offset = epsilon_shift * lightDir;
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

static void drawPauseMenu() {
  ImGui::SetNextWindowPos(
      ImVec2((float)screen_width / 2 - 200, (float)screen_height / 2 - 150),
      ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(400, 300));
  ImGui::Begin("Settings", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

  if (ImGui::BeginTabBar("Settings")) {
    if (ImGui::BeginTabItem("Rendering")) {
      ImGui::ColorEdit3("Intersecting", (float *)&no_intersection_color);
      ImGui::ColorEdit3("Common", (float *)&intersection_color);
      ImGui::SliderFloat("Drawing Range", &drawing_range, 10.0f, 1000.0f);
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Camera")) {
      ImGui::SliderFloat("Camera Speed", &camera_speed_base, 1.0f, 50.0f);
      ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f, "%.1f degrees");
      ImGui::Checkbox("Show Camera's Position", &coordinates);
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Grid")) {
      ImGui::Checkbox("Show X-Grid", &x_grid);
      ImGui::Checkbox("Show Y-Grid", &y_grid);
      ImGui::Checkbox("Show Z-Grid", &z_grid);
      ImGui::Checkbox("Show Full-Grid", &full_grid);
      ImGui::SliderFloat("Grid Range", &grid_range, 10.0f, 1000.0f);
      ImGui::SliderFloat("Full-Grid Step", &full_grid_step, 1.0f, 50.0f);
      ImGui::SliderFloat("Non-Full-Grid Step", &non_full_grid_step, 1.0f,
                         50.0f);
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Light")) {
      ImGui::SliderFloat3("Light Direction", &light_direction.x, -1.0f, 1.0f);
      ImGui::ColorEdit3("Light Color", &light_color.x);
      ImGui::SliderFloat("Light Intensity", &light_intensity, 0.0f, 5.0f);
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("FPS Settings")) {
      ImGui::Checkbox("Show FPS", &show_FPS);
      ImGui::ColorEdit3("FPS Color", (float *)&fps_color);
      ImGui::EndTabItem();
    }
  }

  ImGui::EndTabBar();
  ImGui::End();
}

static void drawCameraInfo() {
  ImGui::SetNextWindowPos(ImVec2(10, 10));
  ImGui::SetNextWindowBgAlpha(0.3f);

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

  if (ImGui::Begin("Camera Coordinates", nullptr, flags)) {
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Camera Position");
    ImGui::Separator();
    ImGui::Text("X: %.2f", camera_pos.x);
    ImGui::Text("Y: %.2f", camera_pos.y);
    ImGui::Text("Z: %.2f", camera_pos.z);
  }

  ImGui::End();
}

static void drawFPS(float fps) {
  ImGui::SetNextWindowPos(ImVec2(screen_width - 120, 10), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(50, 50), ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(0.3f);

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

  if (ImGui::Begin("FPS", nullptr, flags)) {
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "FPS");
    ImGui::Separator();
    ImGui::TextColored(fps_color, "%.1f", fps);
  }

  ImGui::End();
}

static void drawFlag() {
  float flagWidth = 3.0f;
  float flagHeight = 2.0f;
  float stripeHeight = flagHeight / 3.0f;

  glPushMatrix();
  glm::vec3 position = camera_pos + camera_front * flag_distance;
  glTranslatef(position.x, position.y, position.z);
  glRotatef(yaw, 0.0f, 1.0f, 0.0f);
  glRotatef(-pitch, 1.0f, 0.0f, 0.0f);

  float cur_flag_width = flagWidth / 2.0f;
  float cur_flag_height = flagHeight / 2.0f;

  // The white stripe.
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
  glVertex3f(-cur_flag_width, cur_flag_height, 0.0f);
  glVertex3f(cur_flag_width, cur_flag_height, 0.0f);
  glVertex3f(cur_flag_width, cur_flag_height - stripeHeight, 0.0f);
  glVertex3f(-cur_flag_width, cur_flag_height - stripeHeight, 0.0f);
  glEnd();

  // The blue stripe.
  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_QUADS);
  glVertex3f(-cur_flag_width, cur_flag_height - stripeHeight, 0.0f);
  glVertex3f(cur_flag_width, cur_flag_height - stripeHeight, 0.0f);
  glVertex3f(cur_flag_width, cur_flag_height - 2 * stripeHeight, 0.0f);
  glVertex3f(-cur_flag_width, cur_flag_height - 2 * stripeHeight, 0.0f);
  glEnd();

  // The red stripe.
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_QUADS);
  glVertex3f(-cur_flag_width, cur_flag_height - 2 * stripeHeight, 0.0f);
  glVertex3f(cur_flag_width, cur_flag_height - 2 * stripeHeight, 0.0f);
  glVertex3f(cur_flag_width, -cur_flag_height, 0.0f);
  glVertex3f(-cur_flag_width, -cur_flag_height, 0.0f);
  glEnd();

  glPopMatrix();
  glEnable(GL_LIGHTING);
}

void runVisualizer(std::vector<triangle::Triangle<PointTy>> &input,
                   std::map<size_t, size_t> &intersections) {
  if (!glfwInit()) {
    std::cerr << "Error: GLFW was not initialized.";
    return;
  }

  // Getting monitor resolution.
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  screen_width = mode->width;
  screen_height = mode->height;

  GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "3D Scene",
                                        nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    std::cerr << "Error: couldn't create a window.";
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);

  // ImGui settings.
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  // To create the appearance of the dark and light sides, two polygons are
  // specially created for each triangle.
  size_t input_size = input.size();
  std::vector<triangle::Triangle<PointTy>> new_input;
  new_input.reserve(input_size * 2);

  for (size_t i = 0; i < input_size; ++i) {
    new_input.push_back(input[i]);
    new_input.push_back(input[i]);
  }

  size_t triag_num = new_input.size();

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    delta_time = currentFrame - last_frame;
    last_frame = currentFrame;
    float fps = 1.0f / delta_time;

    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (pause)
      drawPauseMenu();

    if (coordinates)
      drawCameraInfo();

    if (show_FPS)
      drawFPS(fps);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(
        glm::radians(fov), (float)screen_width / (float)screen_height, 0.1f,
        drawing_range);
    glm::mat4 view =
        glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
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

    if (show_flag)
      drawFlag();

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
