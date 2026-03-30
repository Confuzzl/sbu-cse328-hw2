#pragma once

#include <glm/glm.hpp>

#include "color.h"

#include <vector>

struct SceneExtra {
  static constexpr auto MAX_ELEMENTS = 256;

  enum struct ElementType { WOOD, FIRE, EARTH, METAL, WATER };
  ElementType getGenerator(const ElementType E);
  ElementType getDestroyer(const ElementType E);

  struct Ball {
    glm::vec2 pos{};
    float rad = 0;
    glm::vec2 vel{};

    Ball() = default;
    Ball(const Ball &) = default;
    Ball(Ball &&) = default;
    Ball &operator=(const Ball &) = default;
    Ball &operator=(Ball &&) = default;

    Ball(const glm::vec2 pos, const float rad, const glm::vec2 vel);

    Ball operator+(const glm::vec2 t) const;
    bool operator==(const Ball &that) const;

    bool intersects(const Ball &that) const;

    glm::vec3 asVec3() const { return {pos, rad}; }
  };

  struct Element : Ball {
    static constexpr auto DEFAULT_RADIUS = 24;
    ElementType type;

    Element(const glm::vec2 pos, const float rad, const glm::vec2 vel,
            const ElementType type);

    glm::vec3 asVec3() const { return {pos, rad}; }

    bool operator==(const Element &that) const;
  };

  std::vector<Element> elements;
  int counts[5]{0};

  struct Config {
    float radius = Element::DEFAULT_RADIUS;
    glm::vec2 velocity{};
    ElementType type = ElementType::WOOD;
  } config{}, changeConfig{};
  glm::vec2 speedRange{50, 100};
  glm::vec2 speedChangeRange{50, 100};
  bool changeKeepVelocity = true;

  enum struct State { ADD, DELETE, CHANGE } state = State::ADD;
  Ball preview{}, previewNext{};
  int hoveringIndex = -1;
  Element *hovering = nullptr;
  bool placeable = false;
  void updateHovering(const glm::ivec2 pos);
  void updatePreview(const glm::ivec2 pos);

  SceneExtra();

  void initialize();
  void update(const float dt);

  void onClick();

  void clear();
  void reset();
};