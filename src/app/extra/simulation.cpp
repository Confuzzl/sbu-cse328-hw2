#include "app/extra/simulation.h"

#include <imgui.h>

#include "app/app.h"

SimulationExtra::SimulationExtra()
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

void SimulationExtra::update(const float dt) { scene.update(dt); }

void SimulationExtra::renderImpl(const float dt) { renderer.render(dt); }
void SimulationExtra::drawSideBarImpl() {
  auto &scene = app().simExtra.scene;
  std::string text;
  static constexpr const char *NAMES[]{"Wood", "Fire", "Earth", "Metal",
                                       "Water"};
  for (int i = 0; i < 5; i++) {
    text = fmt::format("{}: {}", NAMES[i], scene.counts[i]);
    ImGui::Text(text.c_str());
  }
  text = fmt::format("Total: {}", scene.elements.size());
  ImGui::Text(text.c_str());
}