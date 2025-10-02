#pragma once

#include <GLFW/glfw3.h>
#include <map>

#include "../triangles.hpp"

using PointTy = double;

namespace visualizer {
// Main visualizer function - entry point for 3D visualization.
void runVisualizer(std::vector<triangle::Triangle<PointTy>> &input,
                   std::map<size_t, size_t> &intersections);

// Global application settings.
extern unsigned int screen_width;  // Current screen width in pixels
extern unsigned int screen_height; // Current screen height in pixels
extern bool pause;                 // Pause state (true when game is paused)

// Helper functions for window and OpenGL.
GLFWwindow *createWindow();
bool initGLEW();
void mainLoop(GLFWwindow *window,
              std::vector<triangle::Triangle<PointTy>> &input,
              std::map<size_t, size_t> &intersections);

// Clean up resources and shutdown.
void cleanup(GLFWwindow *window);
} // namespace visualizer