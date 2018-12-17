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
 
 
SET (ThirdDepDir  "" CACHE PATH "ThirdDepDir")
if("${ThirdDepDir}" STREQUAL "")
  MESSAGE (FATAL_ERROR "NOT FOUND: ThirdDepDir" )
endif() 
  

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
${CMAKE_CURRENT_BINARY_DIR}  
${ThirdDepDir}/common 
${ThirdDepDir}/glad/include 
${ThirdDepDir}/glfw/include 
${ThirdDepDir}/assimp-3.3.1/include
${ThirdDepDir}/glm-0.9.8
${ThirdDepDir}/stb_image)
 
file(GLOB CXX_FILES *.cpp *.cxx )
file(GLOB HXX_FILES *.h *.hpp )

add_executable(starrysea
 ${CXX_FILES}
 ${HXX_FILES}
 ${ThirdDepDir}/stb_image/stb_image.cpp
 ) 
  
target_link_libraries(starrysea 
${ThirdDepDir}/glad/lib/Debug/glad.lib 
${ThirdDepDir}/glfw/lib/Debug/glfw3.lib 
${ThirdDepDir}/assimp-3.3.1/lib/*.lib
opengl32.lib) 

