#pragma once

#include "gl/glsl_object.h"

struct Renderer3D {
  struct {
  } shaders;

  // Renderer3D();

  void render(const float dt);
};