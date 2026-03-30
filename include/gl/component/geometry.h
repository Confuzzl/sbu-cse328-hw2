#pragma once

#pragma once

#include "gl/component/uniform_macros.h"

#define GEOM_BEGIN(tname, sname)                                               \
  struct tname : UniformHolder {                                               \
    static constexpr char name[] = sname
#define GEOM_END                                                               \
  using UniformHolder::UniformHolder;                                          \
  }

namespace shaders::geom {
GEOM_BEGIN(Element, "element.geom");
NEW_UNIFORM_BLOCK(ViewBlock);
// NEW_UNIFORM(float, radius);
GEOM_END;
} // namespace shaders::geom

#undef GEOM_BEGIN
#undef GEOM_END