#include "app/app.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include <stdexcept>

// #include "app/default/scene.h"
#include "callback.h"
#include "util.h"

Initializer::Initializer()
    : input{{
                {GLFW_KEY_ESCAPE, Key{[](const double) { app().close(); }}},
            },
            [](GLFWwindow *window, int button, int action, int mods) {},
            [](GLFWwindow *window, double xoffset, double yoffset) {}} {
  if (!glfwInit()) {
    throw std::runtime_error{"GLFW FAILED TO INIT"};
  }

  glfwSetErrorCallback(callback::error);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  window = glfwCreateWindow(DEFAULT_DIMENSIONS.x, DEFAULT_DIMENSIONS.y,
                            "Template", nullptr, nullptr);
  if (!window) {
    throw std::runtime_error{"GLFW FAILED TO CREATE WINDOW"};
  }
  // float main_scale =
  //     ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

  // fmt::println("scale = {}", main_scale);

  // glm::vec2 scale;
  // glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &scale.x, &scale.y);
  // fmt::println("scale2 = ({},{})", scale.x, scale.y);

  glfwGetFramebufferSize(window, &framebufferSize.x, &framebufferSize.y);
  glfwGetWindowSize(window, &windowSize.x, &windowSize.y);

  fmt::println("fb=({},{}) ws=({},{})", framebufferSize.x, framebufferSize.y,
               windowSize.x, windowSize.y);

  const auto xScale = static_cast<float>(framebufferSize.x) / windowSize.x,
             yScale = static_cast<float>(framebufferSize.y) / windowSize.y;
  if (xScale != yScale)
    throw std::runtime_error{"window scales didnt match"};
  fmt::println("windowscale={}", xScale);
  windowScale = xScale;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress))) {
    throw std::runtime_error{"GLAD FAILED TO LOAD"};
  }

  glfwSetFramebufferSizeCallback(window, callback::framebufferSize);
  glfwSetWindowSizeCallback(window, callback::windowSize);

  glfwSetCursorPosCallback(window, Cursor::cursorPosCallback);

  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
                                int action, int mods) {
    app().input.keyCallback(window, key, scancode, action, mods);
    app().currentSim->input.keyCallback(window, key, scancode, action, mods);
  });
  glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button,
                                        int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse)
      return;
    app().input.mouseButtonCallback(window, button, action, mods);
    app().currentSim->input.mouseButtonCallback(window, button, action, mods);
  });
  glfwSetScrollCallback(
      window, [](GLFWwindow *window, double xpos, double ypos) {
        // if (ImGui::GetIO().WantCaptureMouse)
        //   return;
        app().input.scrollCallback(window, xpos, ypos);
        app().currentSim->input.scrollCallback(window, xpos, ypos);
      });

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(callback::debug, 0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 450");
  ImGui::StyleColorsDark();
}
Initializer::~Initializer() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // glfwDestroyWindow(window);
  // glfwTerminate();
}

// App::App()
//     : input{{
//           {GLFW_KEY_ESCAPE, [this](const double) { close(); }},
//       }} {}
void App::run() {
  glfwMaximizeWindow(window);

  ImGuiIO &io = ImGui::GetIO();

  simExtra.scene.initialize();

  double prevT = glfwGetTime(), currT = prevT;
  while (!glfwWindowShouldClose(window)) {
    currT = glfwGetTime();
    const float dt = static_cast<float>(currT - prevT);

    glfwPollEvents();
    input.processKeys(dt);
    currentSim->input.processKeys(dt);

    currentSim->update(dt);

    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    currentSim->render(dt);
    glfwSwapBuffers(window);

    prevT = currT;
  }
}
void App::close() { glfwSetWindowShouldClose(window, true); }

App &app() {
  static App instance{};
  return instance;
}
