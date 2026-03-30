#pragma once

#include "app/3d/renderer.h"
#include "app/3d/scene.h"
#include "app/simulation.h"

struct Simulation3D : Simulation {
  Renderer3D renderer;
  Scene3D scene;

  Simulation3D();

  void update(const float dt) override;

private:
  void renderImpl(const float dt) override;
  void drawSideBarImpl() override;
};