cmake_minimum_required(VERSION 2.8)
if (POLICY CMP0020)
  cmake_policy (SET CMP0020 NEW)
endif()

if (POLICY CMP0043)
  cmake_policy (SET CMP0043 NEW)
endif()

if (POLICY CMP0071)
  cmake_policy (SET CMP0071 NEW)
endif()
if (POLICY CMP0053)
  cmake_policy (SET CMP0053 NEW)
endif()
set(CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake"
  ${CMAKE_MODULE_PATH}
)

PROJECT(MyOpenGL)

add_subdirectory(coordinatesystems)
add_subdirectory(glfwwindow)
add_subdirectory(rectangle)
add_subdirectory(simpletexture)
add_subdirectory(triangle)
add_subdirectory(mesh)
add_subdirectory(depthtest)
add_subdirectory(stenciltest)
add_subdirectory(blending)
add_subdirectory(framebuffers)
add_subdirectory(framebuffers_exercise)
add_subdirectory(asteroids_instanced)
