#include "app/extra/renderer.h"

#include "app/app.h"

#if defined(_MSC_VER) && !defined(__clang__) // MSVC
#define UNREACHABLE __assume(false);
#else // GCC, Clang
#define UNREACHABLE __builtin_unreachable();
#endif

static auto &getElementVBO(const SceneExtra::ElementType e) {
  static auto WOOD_VBO = GL::VBO::create<shaders::vertex_layout::circle>(
                  SceneExtra::MAX_ELEMENTS),
              FIRE_VBO = GL::VBO::create<shaders::vertex_layout::circle>(
                  SceneExtra::MAX_ELEMENTS),
              EARTH_VBO = GL::VBO::create<shaders::vertex_layout::circle>(
                  SceneExtra::MAX_ELEMENTS),
              METAL_VBO = GL::VBO::create<shaders::vertex_layout::circle>(
                  SceneExtra::MAX_ELEMENTS),
              WATER_VBO = GL::VBO::create<shaders::vertex_layout::circle>(
                  SceneExtra::MAX_ELEMENTS);
  using E = SceneExtra::ElementType;
  switch (e) {
  case E::WOOD:
    return WOOD_VBO;
  case E::FIRE:
    return FIRE_VBO;
  case E::EARTH:
    return EARTH_VBO;
  case E::METAL:
    return METAL_VBO;
  case E::WATER:
    return WATER_VBO;
  }
  UNREACHABLE;
}
static GL::Texture &getElementTexture(const SceneExtra::ElementType e) {
  static GL::Texture WOOD{"wood.png", GL_NEAREST}, FIRE{"fire.png", GL_NEAREST},
      EARTH{"earth.png", GL_NEAREST}, METAL{"metal.png", GL_NEAREST},
      WATER{"water.png", GL_NEAREST};
  using E = SceneExtra::ElementType;
  switch (e) {
  case E::WOOD:
    return WOOD;
  case E::FIRE:
    return FIRE;
  case E::EARTH:
    return EARTH;
  case E::METAL:
    return METAL;
  case E::WATER:
    return WATER;
  }
  UNREACHABLE;
}

void RendererExtra::render(const float dt) { /*for (const auto &)*/
  // static auto FOO = GL::VBO::create<shaders::vertex_layout::circle>(1);
  // FOO.write(glm::vec3{500, 500, 100});
  // app()
  //     .shaders.element
  //     .bindTexture(getElementTexture(SceneExtra::ElementType::FIRE))
  //     .draw(GL_POINTS, FOO);

  auto &scene = app().simExtra.scene;
  auto &shaders = app().shaders;

  for (const auto &element : scene.elements) {
    getElementVBO(element.type).write(element.asVec3());
  }

  for (int e = 0; e < 5; e++) {
    const auto type = static_cast<SceneExtra::ElementType>(e);
    shaders.element.bindTexture(getElementTexture(type))
        .draw(GL_POINTS, getElementVBO(type));
  }
}