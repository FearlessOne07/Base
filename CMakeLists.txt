cmake_minimum_required(VERSION 3.20)
project("base" "CXX")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Release")

set(HEADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(HEADER_DEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(COPY "${HEADER_SOURCE_DIR}/" DESTINATION ${HEADER_DEST_DIR})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib/src/raylib.h" DESTINATION ${HEADER_DEST_DIR}/raylib)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib/src/raymath.h" DESTINATION ${HEADER_DEST_DIR}/raylib)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib/src/rlgl.h" DESTINATION ${HEADER_DEST_DIR}/raylib)

add_subdirectory(
  "./vendor/raylib/"
)
add_subdirectory(
  "./src"
)
