#include <GL/glew.h>

#include "visualizer/camera.hpp"
#include "visualizer/flag.hpp"
#include "visualizer/grid.hpp"
#include "visualizer/shader.hpp"
#include "visualizer/ui.hpp"
#include "visualizer/visualizer.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace visualizer {

// Global application state variables.
unsigned int screen_width = 0;  // Will be set to monitor resolution
unsigned int screen_height = 0; // Will be set to monitor resolution
bool pause = false;             // Initial state: not paused

// Some local variables.
namespace {
// Timing variables for frame rate calculation and smooth movement.
float delta_time = 0.0f; // Time between current and last frame
float last_frame = 0.0f; // Time of last frame

// System pointers - manage the main components of the application.
ShaderSystem *shaderSystem = nullptr; // Handles triangle rendering and shaders
CameraSystem *cameraSystem = nullptr; // Manages camera movement and view
UISystem *uiSystem = nullptr;         // User interface and settings
GridSystem *gridSystem = nullptr;     // Coordinate grid rendering
FlagSystem *flagSystem = nullptr;     // Flag marker rendering

// Mouse callback wrapper - forwards mouse events to camera system.
void mouseCallbackWrapper(GLFWwindow *window, double xpos, double ypos) {
  if (cameraSystem)
    cameraSystem->mouseCallback(window, xpos, ypos);
}
} // namespace

void runVisualizer(std::vector<triangle::Triangle<PointTy>> &input,
                   std::map<size_t, size_t> &intersections) {
  // Initialize GLFW windowing system.
  if (!glfwInit()) {
    std::cerr << "Error: GLFW was not initialized.";
    return;
  }

  // Get primary monitor resolution for fullscreen window.
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  screen_width = mode->width;
  screen_height = mode->height;

  // Create application window.
  GLFWwindow *window = createWindow();
  if (!window) {
    glfwTerminate();
    return;
  }

  // Initialize GLEW for OpenGL function loading.
  if (!initGLEW()) {
    glfwTerminate();
    return;
  }

  // Create and initialize all subsystem components.
  shaderSystem = new ShaderSystem();
  cameraSystem = new CameraSystem(pause);
  uiSystem = new UISystem();
  gridSystem = new GridSystem();
  flagSystem = new FlagSystem();

  if (!shaderSystem->initialize()) {
    std::cerr << "Failed to initialize shader system" << std::endl;
    cleanup(window);
    return;
  }

  cameraSystem->initialize(screen_width, screen_height);
  uiSystem->initialize(window);
  gridSystem->initialize();
  flagSystem->initialize();

  // Enter main rendering loop.
  mainLoop(window, input, intersections);

  // Clean up when loop exits.
  cleanup(window);
}

// Create and configure the application window.
GLFWwindow *createWindow() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "3D Scene",
                                        nullptr, nullptr);
  if (!window) {
    std::cerr << "Error: couldn't create a window." << std::endl;
    return nullptr;
  }

  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouseCallbackWrapper);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return window;
}

// Initialize GLEW.
bool initGLEW() {
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();

  if (err != GLEW_OK) {
    std::cerr << "GLEW initialization failed: " << glewGetErrorString(err)
              << std::endl;
    return false;
  }

  return true;
}

void mainLoop(GLFWwindow *window,
              std::vector<triangle::Triangle<PointTy>> &input,
              std::map<size_t, size_t> &intersections) {
  // Configure OpenGL rendering state.
  glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D
  glDisable(GL_CULL_FACE); // Show both sides of triangles

  // Prepare triangle data for rendering.
  std::vector<triangle::Triangle<PointTy>> regular_triangles;
  std::vector<triangle::Triangle<PointTy>> intersecting_triangles;

  static glm::vec3 last_regular_color =
      uiSystem->getSettings().no_intersection_color;
  static glm::vec3 last_intersect_color =
      uiSystem->getSettings().intersection_color;

  // Separate triangles into regular and intersecting groups.
  for (size_t i = 0; i < input.size(); ++i) {
    if (intersections.find(i) != intersections.end()) {
      intersecting_triangles.push_back(input[i]);
    } else {
      regular_triangles.push_back(input[i]);
    }
  }

  // Set up vertex buffers with initial colors.
  shaderSystem->setupTriangleBuffers(
      regular_triangles, intersecting_triangles,
      uiSystem->getSettings().no_intersection_color,
      uiSystem->getSettings().intersection_color);

  // Main rendering loop - runs until window should close.
  while (!glfwWindowShouldClose(window)) {
    // Calculate frame timing.
    float currentFrame = glfwGetTime();
    delta_time = currentFrame - last_frame;
    last_frame = currentFrame;
    float fps = 1.0f / delta_time;

    // Process keyboard input for camera movement.
    cameraSystem->processInput(window, delta_time);

    static bool rKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rKeyPressed) {
      flagSystem->toggle();
      rKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
      rKeyPressed = false;

    // Update camera speed from UI settings.
    cameraSystem->setSpeed(uiSystem->getSettings().camera_speed_base);

    // Start new ImGui frame.
    uiSystem->newFrame();

    // Render UI elements.
    uiSystem->render(pause, cameraSystem->getPosition(), fps);

    // Update triangle colors if they changed in UI.
    if (last_regular_color != uiSystem->getSettings().no_intersection_color ||
        last_intersect_color != uiSystem->getSettings().intersection_color) {

      // Recreate buffers with new colors.
      shaderSystem->setupTriangleBuffers(
          regular_triangles, intersecting_triangles,
          uiSystem->getSettings().no_intersection_color,
          uiSystem->getSettings().intersection_color);

      // Update tracked colors.
      last_regular_color = uiSystem->getSettings().no_intersection_color;
      last_intersect_color = uiSystem->getSettings().intersection_color;
    }

    // Clear screen with background color.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Dark blue-green
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get camera transformation matrices.
    glm::mat4 view = cameraSystem->getViewMatrix();
    glm::mat4 projection =
        glm::perspective(glm::radians(uiSystem->getSettings().fov),
                         (float)screen_width / (float)screen_height, 0.1f,
                         uiSystem->getSettings().drawing_range);
    glm::mat4 model = glm::mat4(1.0f);

    // Set transformation matrices and lighting in shader.
    shaderSystem->setMatrices(model, view, projection);
    shaderSystem->setLighting(uiSystem->getSettings().light_direction,
                              uiSystem->getSettings().light_color,
                              uiSystem->getSettings().light_intensity,
                              cameraSystem->getPosition());

    // Render all triangles.
    shaderSystem->render();

    // Update and render grid system.
    gridSystem->setSettings(uiSystem->getSettings().grid_settings);
    gridSystem->render(view, projection, cameraSystem->getPosition(),
                       uiSystem->getSettings().fov,
                       uiSystem->getSettings().drawing_range, screen_width,
                       screen_height);

    // Render flag if visible
    if (flagSystem->isVisible()) {
      flagSystem->render(cameraSystem->getPosition(), cameraSystem->getFront(),
                         cameraSystem->getYaw(), cameraSystem->getPitch(), view,
                         projection, screen_width, screen_height,
                         uiSystem->getSettings().fov,
                         uiSystem->getSettings().drawing_range);
    }

    // Render ImGui interface.
    uiSystem->renderImGui();

    // Swap buffers and process events.
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

// Clean up all allocated resources.
void cleanup(GLFWwindow *window) {
  delete shaderSystem;
  delete cameraSystem;
  delete uiSystem;
  delete gridSystem;
  delete flagSystem;

  if (uiSystem) {
    uiSystem->shutdown();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}
} // namespace visualizer
