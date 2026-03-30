#include "input/key.h"

#include <GLFW/glfw3.h>

Key::Key(const Callback justOn, const Callback holdOn, const Callback justOff)
    : justOn{justOn}, holdOn{holdOn}, justOff{justOff} {}
Key::Key(const Callback on, const Callback off)
    : justOn{on}, holdOn{on}, justOff{off} {}

void Key::react(const int action) {
  switch (action) {
  case GLFW_RELEASE: {
    on = false;
    justUsed = true;
    break;
  }
  case GLFW_PRESS: {
    on = true;
    justUsed = true;
    break;
  }
  case GLFW_REPEAT: {
  }
  }
}

void Key::operator()(const double dt) {
  if (on) {
    if (justUsed) {
      justOn(dt);
      justUsed = false;
    } else {
      holdOn(dt);
    }
  } else {
    if (justUsed) {
      justOff(dt);
      justUsed = false;
    }
  }
}