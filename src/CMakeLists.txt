set(SRC
  "./Game.cpp"
  "./SceneManager.cpp"
  "./EntityManager.cpp"
  "./RenderContextSingleton.cpp"
)
add_library("${PROJECT_NAME}" STATIC "${SRC}")

target_link_libraries("${PROJECT_NAME}"
  PUBLIC
    raylib
)

target_link_directories("${PROJECT_NAME}"
  PUBLIC
    "../lib/"
)

target_include_directories("${PROJECT_NAME}"
  PRIVATE
    "${CMAKE_SOURCE_DIR}/src/include/"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)

target_include_directories("${PROJECT_NAME}"
  PRIVATE
    "./include/"
  PUBLIC
    "../include/"
    "../vendor/raylib/src/"
)
