#pragma once

#include "gl/uniform.h"

#include "color.h"
#include "gl/component/uniform_macros.h"

#define FRAG_COLOR NEW_UNIFORM(Color, frag_color)
#define SAMPLER GL::Sampler sampler{programID, "sampler"}

#define FRAG_BEGIN(tname, sname)                                               \
  struct tname : UniformHolder {                                               \
    static constexpr char name[] = sname
#define FRAG_END                                                               \
  using UniformHolder::UniformHolder;                                          \
  }

namespace shaders::frag {
FRAG_BEGIN(Basic, "basic.frag");
FRAG_COLOR;
FRAG_END;

FRAG_BEGIN(Element, "element.frag");
NEW_SAMPLER;
// NEW_UNIFORM(float, radius);
// NEW_UNIFORM(glm::vec2, center);
FRAG_END;
} // namespace shaders::frag

#undef FRAG_COLOR
#undef SAMPLER
#undef FRAG_BEGIN
#undef FRAG_END