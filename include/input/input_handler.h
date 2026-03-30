#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include <map>

#include "input/key.h"

struct Cursor {
  static glm::vec2 windowPos;
  static glm::vec2 gamePos;

  static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
};

struct InputHandler {
  std::map<int, Key> keys;
  void processKeys(const double dt);
  void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods);

  GLFWmousebuttonfun mouseButtonCallback;

  GLFWscrollfun scrollCallback;

  // void keyCallback(GLFWwindow *window, int key, int scancode, int action,
  //                  int mods);
  // void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
  // void mouseButtonCallback(GLFWwindow *window, int button, int action,
  //                          int mods);
  // void scrollCallback(GLFWwindow *window, double xpos, double ypos);
};