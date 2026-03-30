#include "gl/vertex_layout.h"

namespace shaders::vertex_layout {
template <> void enable<pos>(const GLuint vaoID) { enable<glm::vec2>(vaoID); }
template <> void enable<circle>(const GLuint vaoID) {
  enable<glm::vec2, glm::vec1>(vaoID);
}
// template <> void enable<face>(const GLuint vaoID) {
//   enable<glm::vec2, glm::vec1, glm::/*u*/ vec1>(vaoID);
// }
template <> void enable<postex>(const GLuint vaoID) {
  enable<glm::vec2, glm::vec2>(vaoID);
}
} // namespace shaders::vertex_layout