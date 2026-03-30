#include "app/default/simulation.h"

#include <imgui.h>

#include "app/app.h"

SimulationDefault::SimulationDefault()
    : Simulation{
          {{
               {GLFW_KEY_1, Key{[](const double) {
                  app().simDefault.scene.state = SceneDefault::State::BALL;
                }}},
               {GLFW_KEY_2, Key{[](const double) {
                  app().simDefault.scene.state = SceneDefault::State::DELETE;
                }}},
               {GLFW_KEY_3, Key{[](const double) {
                  app().simDefault.scene.state = SceneDefault::State::FACE;
                }}},
           },
           [](GLFWwindow *window, int button, int action, int mods) {
             if (action == GLFW_PRESS) {
               app().simDefault.scene.onClick();
             }
           },
           [](GLFWwindow *window, double xoffset, double yoffset) {}}} {}

void SimulationDefault::update(const float dt) { scene.update(dt); }

void SimulationDefault::renderImpl(const float dt) { renderer.render(dt); }
void SimulationDefault::drawSideBarImpl() {
  auto &scene = app().simDefault.scene;
  auto foo = fmt::format("balls: ({}B+{}F)/{}", scene.balls.size(),
                         scene.faces.size(), SceneDefault::MAX_BALLS);
  ImGui::Text(foo.c_str());

  ImGui::SeparatorText("Tools");
  struct ButtonData {
    const char *name;
  };
  static const std::map<SceneDefault::State, ButtonData> buttonData{
      {SceneDefault::State::BALL, {"1: Ball"}},
      {SceneDefault::State::DELETE, {"2: Delete"}},
      {SceneDefault::State::FACE, {"3: Face"}},
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

  ImGui::SeparatorText("Ball parameters");
  static int configType = 0;
  ImGui::RadioButton("default", &configType, 0);
  ImGui::SameLine();
  ImGui::RadioButton("custom", &configType, 1);
  scene.configMode = static_cast<SceneDefault::ConfigMode>(configType);
  switch (scene.configMode) {
  case SceneDefault::ConfigMode::DEFAULT: {
    scene.updateDefaultConfig();
    ImGui::BeginDisabled();
    ImGui::InputFloat("radius", &scene.defaultConfig.radius);
    ImGui::InputFloat2("velocity", &scene.defaultConfig.velocity.x);
    ImGui::EndDisabled();
    if (!scene.defaultSuccess) {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ImColor{RED}});
      ImGui::Text("ERROR");
      if (ImGui::IsItemHovered() && ImGui::BeginTooltip()) {
        ImGui::SetTooltip("Couldn't parse etc/config.txt");
        ImGui::EndTooltip();
      }
      ImGui::PopStyleColor();
    }
    break;
  }
  case SceneDefault::ConfigMode::CUSTOM: {
    if (scene.customConfig.radius == 0) { // unset
      scene.customConfig = scene.defaultConfig;
    }
    ImGui::InputFloat("radius", &scene.customConfig.radius);
    ImGui::InputFloat2("velocity", &scene.customConfig.velocity.x);
    break;
  }
  }
}