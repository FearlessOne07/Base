cmake_minimum_required(VERSION 3.20)
project("base" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
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

### GLFW OPTIONS
# Enable Wayland backend in GLFW
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "Build GLFW with Wayland support")

# Disable X11 backend in GLFW
set(GLFW_BUILD_X11 OFF CACHE BOOL "Do not build GLFW with X11 support")

# Opengl Stuff
add_subdirectory("./vendor/glfw/")
add_subdirectory("./vendor/glad/")
add_subdirectory("./vendor/glm/")

## LIBSAMPLE RATE OPTIONS
set(BUILD_TESTING OFF CACHE  BOOL "Remove libsamplerate tests")

## MINAUDIO OPTIONS
set(MINIAUDIO_NO_EXTRA_NODES ON CACHE BOOL "Do not build extra node graph nodes")
set(MINIAUDIO_NO_LIBVORBIS ON CACHE BOOL "Disable miniaudio_libvorbis")
set(MINIAUDIO_NO_LIBOPUS ON CACHE BOOL "Disable miniaudio_libopus")

if(NOT WIN32)
set(MSDFGEN_USE_VCPKG OFF CACHE BOOL "" FORCE)
set(MSDF_ATLAS_USE_VCPKG OFF CACHE BOOL "" FORCE)
endif()

set(MSDFGEN_CORE_ONLY ON CACHE BOOL "" FORCE)
set(MSDFGEN_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(MSDFGEN_BUILD_EXT OFF CACHE BOOL "" FORCE)      # so you get FreeType/PNG/TinyXML2 helpers
set(MSDFGEN_BUILD_LIBRARY ON CACHE BOOL "" FORCE)  # build library
set(MSDFGEN_BUILD_EXE OFF CACHE BOOL "" FORCE)     # don’t need CLI tool
set(MSDF_ATLAS_USE_SKIA OFF CACHE BOOL "" FORCE)

add_subdirectory("./vendor/freetype/")

# Make msdf-atlas-gen see FreeType
if (TARGET freetype AND NOT TARGET Freetype::Freetype)
  add_library(Freetype::Freetype ALIAS freetype)
endif()
add_subdirectory("./vendor/msdf-atlas-gen")


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
