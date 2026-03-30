#include "app/simulation.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "app/app.h"

void Simulation::drawSideBar(const float dt) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::SetNextWindowPos({0, 0});
  ImGui::SetNextWindowSize({250, static_cast<float>(app().windowSize.y)});
  ImGui::SetNextWindowBgAlpha(0.5f);

  ImGui::Begin("sidebar", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

  auto foo = fmt::format("dt = {:.4f}", dt);
  ImGui::Text(foo.c_str());
  foo = fmt::format("pos = {} {}", Cursor::windowPos.x, Cursor::windowPos.y);
  ImGui::Text(foo.c_str());

  if (ImGui::BeginTabBar("tabs")) {
    using P = std::tuple<const char *, Simulation *, ImGuiTabItemFlags>;
    for (const auto [name, sim, flag] : {
             P{"Basic", &app().simDefault, 0},
             {"Extra 2D", &app().simExtra, ImGuiTabItemFlags_SetSelected},
             //{"Extra 3D", &app().sim3D, 0},
         }) {
      if (ImGui::BeginTabItem(name, 0, flag)) {
        app().currentSim = sim;
        drawSideBarImpl();
        ImGui::EndTabItem();
      }
    }
    ImGui::EndTabBar();
  }

  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Simulation::render(const float dt) {
  renderImpl(dt);
  drawSideBar(dt);
}