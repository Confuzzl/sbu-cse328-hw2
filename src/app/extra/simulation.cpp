#include "app/extra/simulation.h"

#include <imgui.h>

#include "app/app.h"

SimulationExtra::SimulationExtra()
    : Simulation{{{
                      {GLFW_KEY_1, Key{[](const double) {
                         app().simExtra.scene.state = SceneExtra::State::ADD;
                       }}},
                      {GLFW_KEY_2, Key{[](const double) {
                         app().simExtra.scene.state = SceneExtra::State::DELETE;
                       }}},
                      {GLFW_KEY_3, Key{[](const double) {
                         app().simExtra.scene.state = SceneExtra::State::CHANGE;
                       }}},
                  },
                  [](GLFWwindow *window, int button, int action, int mods) {
                    if (action == GLFW_PRESS) {
                      app().simExtra.scene.onClick();
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
  ImGui::SameLine();
  ImGui::TextDisabled("(?)");
  if (ImGui::BeginItemTooltip()) {
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted("5 way rock paper scissors.\n"
                           "Generation cycle:\n"
                           "\tWood feeds Fire\n"
                           "\tFire feeds Earth\n"
                           "\tEarth feeds Metal\n"
                           "\tMetal feeds Water\n"
                           "\tWater feeds Wood\n"
                           "Destruction cycle:\n"
                           "\tWood kills Earth\n"
                           "\tFire kills Metal\n"
                           "\tEarth kills Water\n"
                           "\tMetal kills Wood\n"
                           "\tWater kills Fire\n");
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }

  ImGui::SeparatorText("Tools");
  struct ButtonData {
    const char *name;
  };
  static const std::map<SceneExtra::State, ButtonData> buttonData{
      {SceneExtra::State::ADD, {"1: Add"}},
      {SceneExtra::State::DELETE, {"2: Delete"}},
      {SceneExtra::State::CHANGE, {"3: Change"}},
  };
  static constexpr auto button = [](auto name) {
    static constexpr ImVec2 SIZE{200, 75};
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - SIZE.x) / 2);
    return ImGui::Button(name, SIZE);
  };
  for (const auto [state, data] : buttonData) {
    if (scene.state == state) {
      ImGui::BeginDisabled();
      button(data.name);
      ImGui::EndDisabled();
    } else {
      if (button(data.name)) {
        scene.state = state;
      }
    }
  }

  if (button("CLEAR ALL")) {
    scene.clear();
  }
  if (button("RESET")) {
    scene.reset();
  }

  if (scene.state == SceneExtra::State::DELETE)
    return;

  auto &config =
      scene.state == SceneExtra::State::ADD ? scene.config : scene.changeConfig;
  auto &speedRange = scene.state == SceneExtra::State::ADD
                         ? scene.speedRange
                         : scene.speedChangeRange;

  ImGui::Checkbox("Keep velocity", &scene.changeKeepVelocity);
  if (scene.state == SceneExtra::State::ADD || !scene.changeKeepVelocity) {
    ImGui::InputFloat2("Velocity", &config.velocity.x);
    if (ImGui::CollapsingHeader("Random")) {
      ImGui::InputFloat2("Speed", &speedRange.x);
      if (ImGui::Button("Randomize")) {
        config.velocity =
            random_vec2() * random_float(speedRange.x, speedRange.y);
      }
    }
  }

  static int currentType = 0;
  if (ImGui::BeginCombo("Type", NAMES[currentType])) {
    for (int i = 0; i < 5; i++) {
      const bool selected = i == currentType;
      if (ImGui::Selectable(NAMES[i], selected)) {
        currentType = i;
        config.type = static_cast<SceneExtra::ElementType>(i);
      }
      if (selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
}