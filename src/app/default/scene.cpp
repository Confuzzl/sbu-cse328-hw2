#include "app/default/scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "app/app.h"
#include "util.h"

#include <cmath>
#include <filesystem>
#include <fstream>

SceneDefault::Ball::Ball(const glm::vec2 pos, const float rad,
                         const glm::vec2 vel)
    : pos{pos}, rad{rad}, vel{vel} {}
SceneDefault::Ball SceneDefault::Ball::operator+(const glm::vec2 t) const {
  return {pos + t, rad, vel};
}
// bool Scene::Ball::operator==(const Ball &that) const {
//   return pos == that.pos && rad == that.rad && vel == that.vel;
// }

bool SceneDefault::Ball::intersects(const Ball &that) const {
  const float rr = rad + that.rad;
  const float rr2 = rr * rr;
  const float diff2 = glm::distance2(pos, that.pos);
  return diff2 < rr2;
}

// unsigned int SceneDefault::Face::level() const {
//   return std::log2(generation + 1);
// }

SceneDefault::SceneDefault() {
  shapes.reserve(MAX_BALLS);
  balls.reserve(MAX_BALLS);
  faces.reserve(MAX_BALLS);
}

void SceneDefault::updateHovering(const glm::ivec2 pos) {
  hoveringIndex = -1;
  hovering = nullptr;
  for (auto i = 0; i < shapes.size(); i++) {
    const auto &[cpos, rad, vel] = *shapes[i];
    const auto r2 = rad * rad;
    if (glm::distance2(glm::vec2{pos}, cpos) < r2) {
      hoveringIndex = i;
      hovering = shapes[i];
      return;
    }
  }
}
void SceneDefault::tryDeleteGeneric() {
  if (hoveringIndex == -1)
    return;

  if (const auto diff = hovering - balls.data();
      0 <= diff && diff < balls.size()) {
    balls.erase(balls.begin() + diff);
  }
  if (const auto diff = hovering - static_cast<Ball *>(faces.data());
      0 <= diff && diff < faces.size()) {
    faces.erase(faces.begin() + diff);
  }
  shapes.erase(shapes.begin() + hoveringIndex);
}
void SceneDefault::update(const float dt) {
  updatePreview(Cursor::gamePos);
  updateHovering(Cursor::gamePos);

  for (auto ball : shapes) {
    auto &[pos, rad, vel] = *ball;
    const auto step = vel * dt;
    const auto nextPos = pos + step;

    // window checks
    for (int i = 0; i < 2; i++) {
      if (nextPos[i] - rad < 0 || nextPos[i] + rad > app().framebufferSize[i]) {
        vel[i] *= -1;
      }
    }

    for (auto that : shapes) {
      if (ball == that)
        continue;

      if ((*ball + step).intersects(*that)) {
        const auto v1 = vel, x1 = pos;
        const auto v2 = that->vel, x2 = that->pos;

        // https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
        const auto diff = x1 - x2;
        const auto frac = glm::dot(v1 - v2, diff) / glm::length2(diff);
        const auto prod = frac * diff;

        vel = v1 - prod;
        that->vel = v2 + prod;
        ball->onCollision();
        that->onCollision();
      }
    }

    pos += vel * dt;
  }
}

void SceneDefault::updatePreview(const glm::ivec2 pos) {

  preview = configMode == ConfigMode::CUSTOM
                ? Ball{pos, customConfig.radius, customConfig.velocity}
                : Ball{pos, defaultConfig.radius, defaultConfig.velocity};
  if (shapes.size() >= MAX_BALLS) {
    placeable = false;
    return;
  }

  for (int i = 0; i < 2; i++) {
    if (preview.pos[i] - preview.rad < 0 ||
        preview.pos[i] + preview.rad > app().framebufferSize[i]) {
      placeable = false;
      return;
    }
  }
  for (const auto shape : shapes) {
    if (preview.intersects(*shape)) {
      placeable = false;
      return;
    }
  }

  placeable = true;
}
void SceneDefault::tryPlaceBall() {
  if (!placeable)
    return;
  shapes.emplace_back(&balls.emplace_back(preview));
}
void SceneDefault::tryPlaceFace() {
  if (!placeable)
    return;
  shapes.emplace_back(
      &faces.emplace_back(preview.pos, preview.rad, preview.vel));
}

void SceneDefault::onClick() {
  switch (state) {
  case State::BALL: {
    tryPlaceBall();
    break;
  }
  case State::FACE: {
    tryPlaceFace();
    break;
  }
  case State::DELETE: {
    tryDeleteGeneric();
    break;
  }
  }
}

void SceneDefault::clear() {
  shapes.clear();
  balls.clear();
  faces.clear();
}

static constexpr auto CONFIG_PATH = SOURCE_DIR "/etc/config.txt";
static auto lastConfigWrite() {
  return std::filesystem::last_write_time(CONFIG_PATH);
}
void SceneDefault::updateDefaultConfig() {
  static std::filesystem::file_time_type lastWrite{};
  const auto update = lastConfigWrite();
  if (update > lastWrite) {
    print_info("CONFIG FILE UPDATED");

    std::ifstream file{CONFIG_PATH};
    Config config{};
    file >> defaultConfig.radius >> defaultConfig.velocity.x >>
        defaultConfig.velocity.y;
    defaultSuccess = static_cast<bool>(file);
    if (!file) {
      print_err("ERROR READING config.txt");
      defaultConfig = FALLBACK_CONFIG;
    }
    lastWrite = update;
  }
}