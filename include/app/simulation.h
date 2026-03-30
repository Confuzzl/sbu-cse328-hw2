#pragma once

#include "input/input_handler.h"

struct Simulation {
  InputHandler input;

  Simulation(InputHandler &&input) : input{std::move(input)} {}

  void render(const float dt);
  virtual void update(const float dt) = 0;

private:
  virtual void renderImpl(const float dt) = 0;
  virtual void drawSideBarImpl() = 0;
  void drawSideBar(const float dt);
};