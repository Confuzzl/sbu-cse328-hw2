// #include "app/renderer.h"
//
// #include <glm/glm.hpp>
//
// #include "app/app.h"
// #include "color.h"
// #include "gl/gl_object.h"
// #include "gl/glsl_object.h"
// #include "gl/uniform.h"
// #include "gl/vertex_layout.h"
//
// Renderer::Renderer() {
//   glPointSize(8);
//   // glLineWidth(4);
// }
// void Renderer::renderScene() {
//   static auto BALL_VBO =
//       GL::VBO::create<shaders::vertex_layout::circle>(SceneDefault::MAX_BALLS);
//   static auto PREVIEW_VBO =
//   GL::VBO::create<shaders::vertex_layout::circle>(1);
//
//   for (const auto &ball : app().scene.balls) {
//     BALL_VBO.write(ball.asVec3());
//   }
//   glPatchParameteri(GL_PATCH_VERTICES,
//   shaders.circle.tesselation.PATCH_SIZE);
//   shaders.circle.setFragColor(BLACK).draw(GL_PATCHES, BALL_VBO);
//
//   for (const auto &face : app().scene.faces) {
//     auto circleVBO = GL::VBO::create<shaders::vertex_layout::circle>(
//         1 + 2 * face.generation);
//     auto mouthVBO =
//         GL::VBO::create<shaders::vertex_layout::mouth>(face.generation);
//
//     circleVBO.write(face.asVec3());
//     circleVBO.write(
//         glm::vec3{face.pos - glm::vec2{face.rad / 2, 0}, face.rad / 2});
//     circleVBO.write(
//         glm::vec3{face.pos + glm::vec2{face.rad / 2, 0}, face.rad / 2});
//     mouthVBO.write(glm::vec3{face.pos, face.rad});
//
//     int maxEyes = 1;
//     float faceRad = face.rad;
//     float eyeRad = face.rad / 2;
//     float xOffset = 0;
//     for (int i = 2; i <= face.generation; i++) {
//       if (i == 2 * maxEyes) {
//         maxEyes *= 2;
//         xOffset = face.rad - eyeRad;
//         faceRad /= 2;
//         eyeRad /= 2;
//       }
//       circleVBO.write(
//           glm::vec3{face.pos - glm::vec2{xOffset + eyeRad, 0}, eyeRad});
//       circleVBO.write(
//           glm::vec3{face.pos - glm::vec2{xOffset - eyeRad, 0}, eyeRad});
//       mouthVBO.write(glm::vec3{face.pos - glm::vec2{xOffset, 0}, faceRad});
//       xOffset -= 2 * faceRad;
//     }
//
//     glPatchParameteri(GL_PATCH_VERTICES,
//     shaders.circle.tesselation.PATCH_SIZE);
//     shaders.circle.setFragColor(BLACK).draw(GL_PATCHES, circleVBO);
//     glPatchParameteri(GL_PATCH_VERTICES,
//     shaders.mouth.tesselation.PATCH_SIZE);
//     shaders.mouth.setFragColor(BLACK).draw(GL_PATCHES, mouthVBO);
//   }
//
//   if (app().scene.state == SceneDefault::State::DELETE) {
//     if (const auto hover = app().scene.hovering) {
//       PREVIEW_VBO.write(hover->asVec3());
//     }
//     shaders.circle.setFragColor(RED).draw(GL_PATCHES, PREVIEW_VBO);
//   } else {
//     PREVIEW_VBO.write(app().scene.preview.asVec3());
//     shaders.circle.setFragColor(app().scene.placeable ? GREEN : RED)
//         .draw(GL_PATCHES, PREVIEW_VBO);
//   }
// }
// void Renderer::renderSceneExtra() {}
// void Renderer::renderScene3D() {}
// void Renderer::render(const float dt) {
//   switch (app().sceneType) {
//   case App::SceneType::DEFAULT:
//     renderScene();
//     break;
//   case App::SceneType::EXTRA_2D:
//     renderSceneExtra();
//     break;
//   case App::SceneType::EXTRA_3D:
//     renderScene3D();
//     break;
//   }
//
//   gui.render(dt);
// }