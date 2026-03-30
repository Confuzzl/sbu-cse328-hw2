#include "app/default/renderer.h"

#include "app/app.h"
#include "app/default/scene.h"

void RendererDefault::render(const float dt) {
  static auto BALL_VBO =
      GL::VBO::create<shaders::vertex_layout::circle>(SceneDefault::MAX_BALLS);
  static auto PREVIEW_VBO = GL::VBO::create<shaders::vertex_layout::circle>(1);

  auto &scene = app().simDefault.scene;
  auto &shaders = app().shaders;

  for (const auto &ball : scene.balls) {
    BALL_VBO.write(ball.asVec3());
  }
  glPatchParameteri(GL_PATCH_VERTICES, shaders.circle.tesselation.PATCH_SIZE);
  shaders.circle.setFragColor(BLACK).draw(GL_PATCHES, BALL_VBO);

  for (const auto &face : scene.faces) {
    auto circleVBO = GL::VBO::create<shaders::vertex_layout::circle>(
        1 + 2 * face.generation);
    auto mouthVBO =
        GL::VBO::create<shaders::vertex_layout::mouth>(face.generation);

    circleVBO.write(face.asVec3());
    circleVBO.write(
        glm::vec3{face.pos - glm::vec2{face.rad / 2, 0}, face.rad / 2});
    circleVBO.write(
        glm::vec3{face.pos + glm::vec2{face.rad / 2, 0}, face.rad / 2});
    mouthVBO.write(glm::vec3{face.pos, face.rad});

    int maxEyes = 1;
    float faceRad = face.rad;
    float eyeRad = face.rad / 2;
    float xOffset = 0;
    for (int i = 2; i <= face.generation; i++) {
      if (i == 2 * maxEyes) {
        maxEyes *= 2;
        xOffset = face.rad - eyeRad;
        faceRad /= 2;
        eyeRad /= 2;
      }
      circleVBO.write(
          glm::vec3{face.pos - glm::vec2{xOffset + eyeRad, 0}, eyeRad});
      circleVBO.write(
          glm::vec3{face.pos - glm::vec2{xOffset - eyeRad, 0}, eyeRad});
      mouthVBO.write(glm::vec3{face.pos - glm::vec2{xOffset, 0}, faceRad});
      xOffset -= 2 * faceRad;
    }

    glPatchParameteri(GL_PATCH_VERTICES, shaders.circle.tesselation.PATCH_SIZE);
    shaders.circle.setFragColor(BLACK).draw(GL_PATCHES, circleVBO);
    glPatchParameteri(GL_PATCH_VERTICES, shaders.mouth.tesselation.PATCH_SIZE);
    shaders.mouth.setFragColor(BLACK).draw(GL_PATCHES, mouthVBO);
  }

  if (scene.state == SceneDefault::State::DELETE) {
    if (const auto hover = scene.hovering) {
      PREVIEW_VBO.write(hover->asVec3());
    }
    shaders.circle.setFragColor(RED).draw(GL_PATCHES, PREVIEW_VBO);
  } else {
    PREVIEW_VBO.write(scene.preview.asVec3());
    shaders.circle.setFragColor(scene.placeable ? GREEN : RED)
        .draw(GL_PATCHES, PREVIEW_VBO);
  }
}