cmake_minimum_required(VERSION 3.16)
project(hw2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_executable(hw2 
  "src/main.cpp"
  "src/app/app.cpp"
  "src/util.cpp"
  "src/gl/uniform.cpp"
  "src/gl/gl_object.cpp"
  "src/gl/glsl_object.cpp"
  "src/callback.cpp"
  "src/gl/texture.cpp"
  "src/color.cpp"
  "src/gl/vertex_layout.cpp"
  "src/app/renderer.cpp"
  "src/input/key.cpp"
  "src/input/input_handler.cpp"
  "src/app/default/scene.cpp"
  "src/app/extra/scene.cpp"
  "src/app/3d/scene.cpp"
  
  "src/app/simulation.cpp"
  "src/app/default/renderer.cpp"
  "src/app/default/simulation.cpp"
  "src/app/extra/simulation.cpp"
  "src/app/extra/renderer.cpp"
  "src/app/3d/renderer.cpp"
  "src/app/3d/simulation.cpp"

  "include/stb_image.h"
  "include/gl/gl_object.h"
  "include/gl/glsl_object.h"
  "include/gl/uniform.h"
  "include/gl/vao.h"
  "include/gl/vertex_layout.h"
  "include/gl/component/vertex.h"
  "include/gl/component/fragment.h"
  "include/gl/component/tessellation.h"
  "include/gl/texture.h"
  "include/app/app.h"
  "include/app/simulation.h"
  "include/input/key.h"
  "include/input/input_handler.h"
  "include/color.h"
  "include/callback.h"
  
  "include/app/default/renderer.h"
  "include/app/default/scene.h"
  "include/app/default/simulation.h"
  "include/app/extra/renderer.h"
  "include/app/extra/scene.h"
  "include/app/extra/simulation.h"
  "include/app/3d/renderer.h"
  "include/app/3d/scene.h"
  "include/app/3d/simulation.h"
 "include/gl/component/geometry.h"
)
target_include_directories(hw2 PUBLIC include)
target_compile_definitions(hw2 PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

if (WIN32)
  list(APPEND CMAKE_PREFIX_PATH "C:/Program Files (x86)/GLFW/lib/cmake")
endif()
find_package(glfw3 REQUIRED)
target_link_libraries(hw2 glfw)

add_subdirectory("glad")
target_link_libraries(hw2 glad)

add_subdirectory("glm")
target_link_libraries(hw2 glm::glm)

add_subdirectory("imgui")
target_link_libraries(hw2 imgui)

add_subdirectory("fmt")
target_link_libraries(hw2 fmt::fmt)

if (CMAKE_DL_LIBS)
  target_link_libraries(hw2 $ {
    CMAKE_DL_LIBS
  })
endif()