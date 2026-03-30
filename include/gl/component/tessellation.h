#pragma once

#include "gl/component/uniform_macros.h"

#define TESS_BEGIN(tname, psize, cname, ename)                                 \
  struct tname : UniformHolder {                                               \
    static constexpr unsigned int PATCH_SIZE = psize;                          \
    static constexpr char controlName[] = cname;                               \
    static constexpr char evalName[] = ename
#define TESS_END                                                               \
  using UniformHolder::UniformHolder;                                          \
  }

namespace shaders::tess {
TESS_BEGIN(Circle, 1, "circle.tesc", "circle.tese");
NEW_UNIFORM_BLOCK(ViewBlock);
TESS_END;

TESS_BEGIN(Mouth, 1, "mouth.tesc", "mouth.tese");
NEW_UNIFORM_BLOCK(ViewBlock);
TESS_END;
} // namespace shaders::tess

#undef TESS_BEGIN
#undef TESS_END