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
