project (hmd)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 11)

find_package(HIDAPI REQUIRED)
if(HIDAPI_FOUND)
  include_directories(${HIDAPI_INCLUDE_DIRS})
  link_libraries(${HIDAPI_LIBRARIES})
else()
    message("HIDAPI not found!")
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_libraries(${GLEW_LIBRARIES})
else()
    message("GLEW not found!")
endif()

find_package(ASSIMP REQUIRED)
if (ASSIMP_FOUND)
  include_directories(${ASSIMP_INCLUDE_DIRS})
  link_libraries(${ASSIMP_LIBRARIES})
else()
    message("ASSIMP not found!")
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
  link_libraries(${OPENGL_LIBRARIES})
else()
    message("OpenGL not found!")
endif()

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GLFW REQUIRED glfw3)
if(GLFW_FOUND)
    include_directories( ${GLFW_INCLUDE_PATH})
else()
    message("GLFW not found!")
endif()

FILE(
  GLOB_RECURSE
  src_mesh
  src/*
  )

include_directories("$ENV{HOME}/OpenHMD/include" ${CMAKE_BINARY_DIR}/include ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
link_directories(${CMAKE_BINARY_DIR})
add_executable(openglexample ${src_mesh})
target_link_libraries(openglexample PRIVATE ${HIDAPI_LIBRARIES} "$ENV{HOME}/OpenHMD/libopenhmd.a" pthread m ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARIES})
