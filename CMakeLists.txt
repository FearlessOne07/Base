cmake_minimum_required(VERSION 3.20)
project("Base" "CXX")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_BUILD_TYPE "Debug")
add_subdirectory(
  "${CMAKE_SOURCE_DIR}/src"
)
add_subdirectory(
  "${CMAKE_SOURCE_DIR}/vendor/raylib"
)
include_directories(
  "./src"
)

