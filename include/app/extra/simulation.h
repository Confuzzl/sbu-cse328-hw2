#pragma once

#include "app/extra/renderer.h"
#include "app/extra/scene.h"
#include "app/simulation.h"

struct SimulationExtra : Simulation {
  RendererExtra renderer;
  SceneExtra scene;

  SimulationExtra();

  void update(const float dt) override;

private:
  void renderImpl(const float dt) override;
  void drawSideBarImpl() override;
};