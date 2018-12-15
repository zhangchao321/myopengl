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

add_executable(blinnphong
 ${CXX_FILES}
 ${HXX_FILES}
 ${ThirdDepDir}/stb_image/stb_image.cpp
 ) 
  
target_link_libraries(blinnphong 
${ThirdDepDir}/glad/lib/Debug/glad.lib 
${ThirdDepDir}/glfw/lib/Debug/glfw3.lib 
${ThirdDepDir}/assimp-3.3.1/lib/*.lib
opengl32.lib) 

