#include "visualizer/ui.hpp"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "visualizer/visualizer.hpp"

namespace visualizer {

void UISystem::initialize(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // Initialize default colors for triangles.
  settings.no_intersection_color =
      glm::vec3(27.0f / 255.0f, 94.0f / 255.0f, 215.0f / 255.0f); // Blue.
  settings.intersection_color =
      glm::vec3(215.0f / 255.0f, 27.0f / 255.0f, 27.0f / 255.0f); // Red.

  // Initialize default grid settings.
  settings.grid_settings.x_grid = false;    // X-plane grid disabled by default
  settings.grid_settings.y_grid = true;     // Y-plane grid enabled (floor)
  settings.grid_settings.z_grid = false;    // Z-plane grid disabled
  settings.grid_settings.full_grid = false; // 3D grid disabled
  settings.grid_settings.range = 250.0f;    // Grid extends 250 units
  settings.grid_settings.full_step = 10.0f; // Step for 3D grid
  settings.grid_settings.non_full_step = 1.0f; // Step for plane grids
}

void UISystem::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

// Render UI elements based on current state and settings
void UISystem::render(bool pause, const glm::vec3 &camera_pos, float fps) {
  if (pause)
    drawPauseMenu(); // Show settings menu when paused.

  if (settings.show_coordinates)
    drawCameraInfo(camera_pos); // Show camera position overlay.

  if (settings.show_fps)
    drawFPS(fps); // Show FPS counter overlay.
}

void UISystem::renderImGui() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UISystem::shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

// Draw the main settings menu (shown when game is paused).
void UISystem::drawPauseMenu() {
  // Position window in center of screen.
  ImGui::SetNextWindowPos(
      ImVec2((float)screen_width / 2 - 200, (float)screen_height / 2 - 150),
      ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(400, 300));
  ImGui::Begin("Settings", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

  // Tabbed interface for different setting categories.
  if (ImGui::BeginTabBar("Settings")) {

    // Rendering settings tab.
    if (ImGui::BeginTabItem("Rendering")) {
      ImGui::ColorEdit3("Non-intersecting",
                        (float *)&settings.no_intersection_color);
      ImGui::ColorEdit3("Intersecting", (float *)&settings.intersection_color);
      ImGui::SliderFloat("Drawing Range", &settings.drawing_range, 10.0f,
                         1000.0f);
      ImGui::EndTabItem();
    }

    // Camera settings tab.
    if (ImGui::BeginTabItem("Camera")) {
      ImGui::SliderFloat("Camera Speed", &settings.camera_speed_base, 1.0f,
                         50.0f);
      ImGui::SliderFloat("FOV", &settings.fov, 30.0f, 120.0f, "%.1f degrees");
      ImGui::Checkbox("Show Camera's Position", &settings.show_coordinates);
      ImGui::EndTabItem();
    }

    // Grid settings tab.
    if (ImGui::BeginTabItem("Grid")) {
      ImGui::Checkbox("Show X-Grid", &settings.grid_settings.x_grid);
      ImGui::Checkbox("Show Y-Grid", &settings.grid_settings.y_grid);
      ImGui::Checkbox("Show Z-Grid", &settings.grid_settings.z_grid);
      ImGui::Checkbox("Show Full-Grid", &settings.grid_settings.full_grid);
      ImGui::SliderFloat("Grid Range", &settings.grid_settings.range, 10.0f,
                         400.0f);
      ImGui::SliderFloat("Full-Grid Step", &settings.grid_settings.full_step,
                         1.0f, 50.0f);
      ImGui::SliderFloat("Non-Full-Grid Step",
                         &settings.grid_settings.non_full_step, 1.0f, 50.0f);
      ImGui::EndTabItem();
    }

    // Lighting settings tab.
    if (ImGui::BeginTabItem("Light")) {
      ImGui::SliderFloat3("Light Direction", &settings.light_direction.x, -1.0f,
                          1.0f);
      ImGui::ColorEdit3("Light Color", &settings.light_color.x);
      ImGui::SliderFloat("Light Intensity", &settings.light_intensity, 0.0f,
                         5.0f);
      ImGui::EndTabItem();
    }

    // FPS display settings tab.
    if (ImGui::BeginTabItem("FPS Settings")) {
      ImGui::Checkbox("Show FPS", &settings.show_fps);
      ImGui::ColorEdit3("FPS Color", (float *)&settings.fps_color);
      ImGui::EndTabItem();
    }
  }

  ImGui::EndTabBar();
  ImGui::End();
}

// Draw camera position overlay in top-left corner.
void UISystem::drawCameraInfo(const glm::vec3 &camera_pos) {
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

// Draw FPS counter overlay in top-right corner.
void UISystem::drawFPS(float fps) {
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
    ImGui::TextColored(settings.fps_color, "%.1f", fps);
  }

  ImGui::End();
}
} // namespace visualizer
