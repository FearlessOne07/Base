cmake_minimum_required(VERSION 3.20)
project("base" "CXX")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Release")

option(BASE_BUILD_DEBUG "Build type for the library" ON)
option(BASE_BUILD_SHARED_LIB "Build Base as a shared library" OFF)

set(BASE_BUILD_SHARED_LIB "${BASE_BUILD_SHARED_LIB}" CACHE BOOL "Whether to build the base library as shared")

if(BASE_BUILD_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
else()
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(BASE_BUILD_SHARED_LIB)
  set(BUILD_SHARED_LIBS ON)
else()
  set(BUILD_SHARED_LIBS OFF)
endif()

set(ROOT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "The root directory")
message(STATUS "Root Base directory: ${ROOT_BASE_DIR}")

# Headers
set(HEADER_SOURCE_DIR "${ROOT_BASE_DIR}/include")
set(HEADER_DEST_DIR "${ROOT_BASE_DIR}/include")

message(STATUS "Base headers location ${ROOT_BASE_DIR}/include")

file(COPY "${HEADER_SOURCE_DIR}/" DESTINATION ${HEADER_DEST_DIR})
file(COPY "${ROOT_BASE_DIR}/vendor/raylib/src/raylib.h" DESTINATION ${HEADER_DEST_DIR}/raylib)
file(COPY "${ROOT_BASE_DIR}/vendor/raylib/src/raymath.h" DESTINATION ${HEADER_DEST_DIR}/raylib)
file(COPY "${ROOT_BASE_DIR}/vendor/raylib/src/rlgl.h" DESTINATION ${HEADER_DEST_DIR}/raylib)

add_subdirectory(
  "./vendor/raylib/"
)
add_subdirectory(
  "./src"
)
