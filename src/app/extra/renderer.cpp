#include "app/extra/renderer.h"

#include "app/app.h"

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
static Color getElementColor(const SceneExtra::ElementType e) {
  using E = SceneExtra::ElementType;
  switch (e) {
  case E::WOOD:
    return GREEN;
  case E::FIRE:
    return RED;
  case E::EARTH:
    return 0x804000_rgb;
  case E::METAL:
    return YELLOW;
  case E::WATER:
    return BLUE;
  }
  UNREACHABLE;
}

static Color getComplementary(const Color c) {
  const auto max = std::max(c.r, std::max(c.g, c.b));
  const auto min = std::min(c.r, std::min(c.g, c.b));
  const auto sum = max + min;
  return Color{sum - c.r, sum - c.g, sum - c.b};
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

  static auto PREVIEW_VBO = GL::VBO::create<shaders::vertex_layout::circle>(1);
  switch (scene.state) {
  case SceneExtra::State::ADD: {
    glPatchParameteri(GL_PATCH_VERTICES, shaders::tess::Circle::PATCH_SIZE);

    PREVIEW_VBO.write(scene.previewNext.asVec3());
    shaders.circle.setFragColor(BLACK).draw(GL_PATCHES, PREVIEW_VBO);

    PREVIEW_VBO.write(scene.preview.asVec3());
    shaders.circle.setFragColor(getElementColor(scene.config.type))
        .draw(GL_PATCHES, PREVIEW_VBO);

    break;
  }
  case SceneExtra::State::CHANGE:
  case SceneExtra::State::DELETE: {
    const auto hover = scene.hovering;
    if (hover) {
      PREVIEW_VBO.write(hover->asVec3());
      shaders.circle
          .setFragColor(getComplementary(getElementColor(hover->type)))
          .draw(GL_PATCHES, PREVIEW_VBO);
    }
    break;
  }
  }
}