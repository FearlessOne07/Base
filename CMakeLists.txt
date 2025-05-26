cmake_minimum_required(VERSION 3.20)
project("base" "CXX")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BASE_BUILD_DEBUG "Build type for the library" ON)
if(BASE_BUILD_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
else()
  set(CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "Base Build Type: ${CMAKE_BUILD_TYPE}")

set(ROOT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "The root directory")
message(STATUS "Root Base directory: ${ROOT_BASE_DIR}")

# Headers
set(HEADER_SOURCE_DIR "${ROOT_BASE_DIR}/include")
set(HEADER_DEST_DIR "${ROOT_BASE_DIR}/include")

message(STATUS "Base headers location ${ROOT_BASE_DIR}/include")

# Enable Wayland backend in GLFW
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "Build GLFW with Wayland support")

# Disable X11 backend in GLFW
set(GLFW_BUILD_X11 OFF CACHE BOOL "Do not build GLFW with X11 support")

# Set the graphics API (assuming this is a custom variable)
set(GRAPHICS GRAPHICS_API_OPENGL_43 CACHE STRING "Select the graphics API")

# Disable RAylib Audio Support
set(CUSTOMIZE_BUILD ON CACHE BOOL "Allow Build Customizatio")
set(SUPPORT_MODULE_RAUDIO OFF CACHE BOOL "Disable audio device support in raylib")

add_subdirectory(
  "./vendor/raylib/"
)

add_subdirectory(
  "./vendor/portaudio/"
)

add_subdirectory(
  "./vendor/miniaudio"
)

add_subdirectory(
  "./vendor/libsamplerate/"
)

add_subdirectory(
  "./src"
)
