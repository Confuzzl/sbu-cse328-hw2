#pragma once

#include "gl/uniform.h"

#include "gl/component/uniform_macros.h"
#include "gl/vertex_layout.h"

#define VERT_BEGIN(tname, layout, sname)                                       \
  struct tname : Base<layout> {                                                \
    static constexpr char name[] = sname
#define VERT_END                                                               \
  using Base::Base;                                                            \
  }

namespace shaders::vert {
template <typename VertexLayout> struct Base : UniformHolder {
  using LayoutType = VertexLayout;

  using UniformHolder::UniformHolder;
};

VERT_BEGIN(Basic, vertex_layout::pos, "basic.vert");
NEW_UNIFORM_BLOCK(ViewBlock);
VERT_END;

VERT_BEGIN(Identity, vertex_layout::circle, "identity.vert");
VERT_END;

VERT_BEGIN(Circle, vertex_layout::circle, "circle.vert");
VERT_END;

VERT_BEGIN(Mouth, vertex_layout::mouth, "mouth.vert");
VERT_END;

// VERT_BEGIN(Face, vertex_layout::face, "face.vert");
// VERT_END;
} // namespace shaders::vert

#undef VERT_BEGIN
#undef VERT_END