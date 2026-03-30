#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <type_traits>

#include "util.h"

namespace shaders::vertex_layout {
namespace detail {
// template <typename T>
// void attribFormatDispatch(const GLuint vaoID, const GLuint attrIndex,
//                           const GLint size, const GLenum type,
//                           const GLint offset);
// template <typename Integer,
//          std::enable_if_t<std::is_integral_v<Integer>, bool> = true>
// void attribFormatDispatch(const GLuint vaoID, const GLuint attrIndex,
//                          const GLint size, const GLenum type,
//                          const GLint offset) {
//  static_assert(false, "doesnt seem to work");
//  glVertexArrayAttribIFormat(vaoID, attrIndex, size, type, offset);
//}
template <typename Float,
          std::enable_if_t<std::is_same_v<Float, GLfloat>, bool> = true>
void attribFormatDispatch(const GLuint vaoID, const GLuint attrIndex,
                          const GLint size, const GLenum type,
                          const GLint offset) {
  glVertexArrayAttribFormat(vaoID, attrIndex, size, type, false, offset);
}
// template <typename Double,
//           std::enable_if_t<std::is_same_v<Double, GLdouble>, bool> = true>
// void attribFormatDispatch(const GLuint vaoID, const GLuint attrIndex,
//                           const GLint size, const GLenum type,
//                           const GLint offset) {
//   glVertexArrayAttribLFormat(vaoID, attrIndex, size, type, offset);
// }

template <typename T>
void enable_helper(const GLuint vaoID, GLuint &attrIndex, GLint &offset) {
  glEnableVertexArrayAttrib(vaoID, attrIndex);
  attribFormatDispatch<typename T::value_type>(
      vaoID, attrIndex, T::length(), GL::macroOf<typename T::value_type>(),
      offset);
  glVertexArrayAttribBinding(vaoID, attrIndex, 0);
  attrIndex++;
  offset += sizeof(T);
}
} // namespace detail

template <typename... VertexTypes> void enable(const GLuint vaoID) {
  GLuint attrIndex = 0;
  GLint offset = 0;
  (detail::enable_helper<VertexTypes>(vaoID, attrIndex, offset), ...);
}

struct alignas(4) pos {
  glm::vec2 _pos;

  /*constexpr*/ pos() = default;
  template <typename X, typename Y>
  ///*constexpr*/ pos(const X x, const Y y)
  //    : _pos{static_cast<float>(x), static_cast<float>(y)} {}
  /*constexpr*/ pos(const glm::vec2 pos) : _pos{pos} {}

  const void *data() const { return &_pos; }
};
template <> void enable<pos>(const GLuint vaoID);

struct alignas(4) circle {
  glm::vec2 _pos;
  float radius;

  circle() = default;
  // template <typename X, typename Y, typename Z>
  // circle(const X x, const Y y, const Z z)
  //     : _pos{static_cast<float>(x), static_cast<float>(y)},
  //       radius{static_cast<float>(z)} {}
  circle(const glm::vec2 pos, const float radius) : _pos{pos}, radius{radius} {}

  const void *data() const { return &_pos; }
};
template <> void enable<circle>(const GLuint vaoID);

using mouth = circle;

// struct alignas(4) face {
//   glm::vec2 _pos;
//   float radius;
//   /*unsigned int*/ float generation;
//
//   face() = default;
//   face(const glm::vec2 pos, const float radius,
//        const /*unsigned int*/ float generation)
//       : _pos{pos}, radius{radius}, generation{generation} {}
//
//   const void *data() const { return &_pos; }
// };
// template <> void enable<face>(const GLuint vaoID);

struct alignas(4) postex {
  glm::vec2 pos;
  glm::vec2 tex;
  postex(const glm::vec2 pos, const glm::vec2 tex) : pos{pos}, tex{tex} {}

  const void *data() const { return &pos; }
};

template <> void enable<postex>(const GLuint vaoID);
} // namespace shaders::vertex_layout