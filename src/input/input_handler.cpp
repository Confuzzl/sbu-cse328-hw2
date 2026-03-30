#include "input/input_handler.h"

#include <imgui.h>

#include <algorithm>

#include "app/app.h"

glm::vec2 Cursor::windowPos{};
glm::vec2 Cursor::gamePos{};

void Cursor::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  const auto windowSize = app().windowSize;
  const auto y = std::clamp<double>(ypos, 0, windowSize.y);
  windowPos = {std::clamp<double>(xpos, 0, windowSize.x) * app().windowScale,
               y * app().windowScale};
  gamePos = {windowPos.x, (windowSize.y - y) * app().windowScale};
}

// InputHandler::InputHandler()
//     : keys{
//           {GLFW_KEY_ESCAPE, Key{[](const double) { app().close(); }}},
//           //{GLFW_KEY_1, Key{[](const double) {
//
//           //   app().scene.state = SceneDefault::State::BALL;
//           // }}},
//           //{GLFW_KEY_2, Key{[](const double) {
//           //   app().scene.state = SceneDefault::State::DELETE;
//           // }}},
//           //{GLFW_KEY_3, Key{[](const double) {
//           //   app().scene.state = SceneDefault::State::FACE;
//           // }}},
//       } {}

void InputHandler::processKeys(const double dt) {
  for (auto &[keycode, key] : keys)
    key(dt);
}
void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode,
                               int action, int mods) {
  if (const auto iterator = keys.find(key); iterator != keys.cend())
    iterator->second.react(action);
}
// glm::ivec2 InputHandler::worldCursorPos() const {
//   return {cursorPos.x, app().windowSize.y * app().windowScale - cursorPos.y};
// }
// void InputHandler::cursorPosCallback(GLFWwindow *window, double xpos,
//                                      double ypos) {
//   cursorPos = {
//       std::clamp<double>(xpos, 0, app().windowSize.x) * app().windowScale,
//       std::clamp<double>(ypos, 0, app().windowSize.y) * app().windowScale};
// }
// void InputHandler::mouseButtonCallback(GLFWwindow *window, int button,
//                                        int action, int mods) {
//   if (ImGui::GetIO().WantCaptureMouse)
//     return;
//   // if (action == GLFW_PRESS) {
//   //   switch (app().scene.state) {
//   //   case SceneDefault::State::BALL: {
//   //     app().scene.tryPlaceBall();
//   //     break;
//   //   }
//   //   case SceneDefault::State::FACE: {
//   //     app().scene.tryPlaceFace();
//   //     break;
//   //   }
//   //   case SceneDefault::State::DELETE: {
//   //     app().scene.tryDeleteGeneric();
//   //     break;
//   //   }
//   //   }
//   // }
// }
// void InputHandler::scrollCallback(GLFWwindow *window, double xpos,
//                                   double ypos) {}
