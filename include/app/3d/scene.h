#pragma once

#include <glm/glm.hpp>

#include "color.h"

#include <vector>

struct Scene3D {
  enum struct ElementType { WOOD, FIRE, EARTH, METAL, WATER };
  ElementType generator(const ElementType E);
  ElementType destroys(const ElementType E);
  Color elementColor(const ElementType E);

  struct Element {
    glm::vec2 pos;
    float radius;
    glm::vec2 vel;

    ElementType type;
  };

  std::vector<Element> elements;

  void update(const float dt);
};