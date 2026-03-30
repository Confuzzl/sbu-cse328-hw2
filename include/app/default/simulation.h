#pragma once

#include "app/default/renderer.h"
#include "app/default/scene.h"
#include "app/simulation.h"

struct SimulationDefault : Simulation {
  RendererDefault renderer;
  SceneDefault scene;

  SimulationDefault();

  void update(const float dt) override;

private:
  void renderImpl(const float dt) override;
  void drawSideBarImpl() override;
};