#include "app/extra/scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "app/app.h"

SceneExtra::Ball::Ball(const glm::vec2 pos, const float rad,
                       const glm::vec2 vel)
    : pos{pos}, rad{rad}, vel{vel} {}
SceneExtra::Ball SceneExtra::Ball::operator+(const glm::vec2 t) const {
  return {pos + t, rad, vel};
}
bool SceneExtra::Ball::intersects(const Ball &that) const {
  const float rr = rad + that.rad;
  const float rr2 = rr * rr;
  const float diff2 = glm::distance2(pos, that.pos);
  return diff2 < rr2;
}

SceneExtra::ElementType SceneExtra::getGenerator(const ElementType E) {
  switch (E) {
  case ElementType::WOOD:
    return ElementType::WATER;
  case ElementType::FIRE:
    return ElementType::WOOD;
  case ElementType::EARTH:
    return ElementType::FIRE;
  case ElementType::METAL:
    return ElementType::EARTH;
  case ElementType::WATER:
    return ElementType::METAL;
  }
}
SceneExtra::ElementType SceneExtra::getDestroyer(const ElementType E) {
  switch (E) {
  case ElementType::WOOD:
    return ElementType::METAL;
  case ElementType::FIRE:
    return ElementType::WATER;
  case ElementType::EARTH:
    return ElementType::WOOD;
  case ElementType::METAL:
    return ElementType::FIRE;
  case ElementType::WATER:
    return ElementType::EARTH;
  }
}

bool SceneExtra::Ball::operator==(const Ball &that) const {
  return pos == that.pos && rad == that.rad && vel == that.vel;
}

SceneExtra::Element::Element(const glm::vec2 pos, const float rad,
                             const glm::vec2 vel, const ElementType type)
    : Ball{pos, rad, vel}, type{type} {}
bool SceneExtra::Element::operator==(const Element &that) const {
  return Ball::operator==(that) && type == that.type;
}

SceneExtra::SceneExtra() { elements.reserve(MAX_ELEMENTS); }
void SceneExtra::initialize() {
  static constexpr auto NUM_PER_ELEMENT = 8;
  const float radius = app().framebufferSize.y / 2 - 50;
  // const float ratio = static_cast<float>(app().framebufferSize.x) /
  // app().framebufferSize.y;
  const glm::vec2 offset = app().framebufferSize / 2;
  static constexpr float thetaStep = 2 * 3.141592 / (5 * NUM_PER_ELEMENT);
  float theta = 0;
  for (int e = 0; e < 5; e++) {
    counts[e] = NUM_PER_ELEMENT;
    const auto type = static_cast<ElementType>(e);
    for (int i = 0; i < NUM_PER_ELEMENT; i++) {
      elements.emplace_back(
          glm::vec2{std::cos(theta), std::sin(theta)} * radius + offset, 32,
          random_vec2() * random_float(50.0f, 100.0f), type);

      theta += thetaStep;
    }
  }

  // elements.emplace_back(glm::vec2{0, 0}, 64, glm::vec2{5, 5},
  //                       ElementType::EARTH);
}

void SceneExtra::update(const float dt) {
  for (auto &element : elements) {
    auto &pos = element.pos, &vel = element.vel;
    const auto rad = element.rad;

    const auto step = vel * dt;

    const auto nextPos = pos + step;

    // window checks
    for (int i = 0; i < 2; i++) {
      // i = 0 left and right
      // i = 1 top and bottom
      const glm::vec2 positiveNormal{1 - i, i};
      const float dot = glm::dot(vel, positiveNormal);
      if ((nextPos[i] - rad < 0 && dot < 0) || // left and bottom
          (nextPos[i] + rad > app().framebufferSize[i] &&
           dot > 0) // top and right
      ) {
        vel[i] *= -1;
      }
    }

    for (auto &that : elements) {
      if (element == that)
        continue;

      // push them out of each other
      const float rr = element.rad + that.rad;
      const glm::vec2 dist = that.pos - element.pos;
      const float diff = glm::length(dist) - rr;
      if (diff < 0) {
        static constexpr auto PUSH_MULTIPLIER = 1.1f;
        element.pos -= diff * PUSH_MULTIPLIER / 2;
        that.pos += diff * PUSH_MULTIPLIER / 2;
      }

      if ((element + step).intersects(that)) {
        const auto v1 = vel, x1 = pos;
        const auto v2 = that.vel, x2 = that.pos;

        // https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
        const auto diff = x1 - x2;
        const auto frac = glm::dot(v1 - v2, diff) / glm::length2(diff);
        const auto prod = frac * diff;

        if (getGenerator(element.type) == that.type ||
            getDestroyer(that.type) ==
                element.type) { // that generates this OR this destroys that
          counts[static_cast<int>(element.type)]++;
          counts[static_cast<int>(that.type)]--;
          that.type = element.type;
        } else if (getDestroyer(element.type) == that.type ||
                   getGenerator(that.type) ==
                       element
                           .type) // that destroys this OR this generates that
        {
          counts[static_cast<int>(element.type)]--;
          counts[static_cast<int>(that.type)]++;
          element.type = that.type;
        }
        vel = v1 - prod;
        that.vel = v2 + prod;
      }
    }

    pos += step;
  }
}