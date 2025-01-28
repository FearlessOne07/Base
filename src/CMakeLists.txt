set(SRC
  "./base/Game.cpp"
  "./base/SceneManager.cpp"
  "./base/EntityManager.cpp"
  "./base/RenderContextSingleton.cpp"
  "./base/Scene.cpp"
  "./base/RenderContext.cpp"
  "./base/System.cpp"
  "./base/Entity.cpp"
  "./base/SystemManager.cpp"
  "./base/AssetManager.cpp"
)
add_library("${PROJECT_NAME}" "${SRC}")

target_compile_definitions(base PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<BOOL:${BUILD_SHARED_LIBS}>:BASE_BUILD_LIBTYPE_SHARED>
)

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
