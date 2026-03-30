#include "app/3d/simulation.h"

#include <imgui.h>

#include "app/app.h"

Simulation3D ::Simulation3D()
    : Simulation{{{
                      {GLFW_KEY_1, Key{[](const double) {}}},
                      {GLFW_KEY_2, Key{[](const double) {}}},
                      {GLFW_KEY_3, Key{[](const double) {}}},
                  },
                  [](GLFWwindow *window, int button, int action, int mods) {
                    if (action == GLFW_PRESS) {
                    }
                  },
                  [](GLFWwindow *window, double xoffset, double yoffset) {}}} {}

void Simulation3D::update(const float dt) {}

void Simulation3D::renderImpl(const float dt) {}
void Simulation3D::drawSideBarImpl() { ImGui::Text("3d"); }