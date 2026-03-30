#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "app/3d/simulation.h"
#include "app/default/simulation.h"
#include "app/extra/simulation.h"
#include "app/simulation.h"
#include "gl/glsl_object.h"
#include "input/input_handler.h"

struct Initializer {
  GLFWwindow *window = nullptr;
  static constexpr glm::ivec2 DEFAULT_DIMENSIONS{1280, 720};
  float windowScale;
  glm::ivec2 framebufferSize;
  glm::ivec2 windowSize;

  InputHandler input;

  Initializer();
  ~Initializer();
};
struct App : Initializer {
  struct {
    shaders::Basic basic;
    shaders::Circle circle;
    shaders::Mouth mouth;
    shaders::Element element;
  } shaders;

  SimulationDefault simDefault;
  SimulationExtra simExtra;
  // Simulation3D sim3D;

  Simulation *currentSim = &simDefault;

  // App();

  void run();
  void close();
};

App &app();
