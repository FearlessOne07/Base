set(SRC
  "./base/Game.cpp"
  "./base/SceneManager.cpp"
  "./base/EntityManager.cpp"
  "./base/RenderContextSingleton.cpp"
  "./base/Scene.cpp"
  "./base/RenderContext.cpp"
)
add_library("${PROJECT_NAME}" STATIC "${SRC}")

target_link_libraries("${PROJECT_NAME}"
  PUBLIC
    raylib
)

target_link_directories("${PROJECT_NAME}"
  PRIVATE
    "${ROOT_BASE_DIR}/build/vendor/raylib"
)

target_include_directories("${PROJECT_NAME}"
  PRIVATE
    "${ROOT_BASE_DIR}/src/include/"
    "${ROOT_BASE_DIR}/include/"
)
