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
  "./base/Component.cpp"
  "./base/EventBus.cpp"

  "./systems/MoveSystem.cpp"
  "./systems/RenderSystem.cpp"
  "./systems/InputSystem.cpp"
  "./systems/CameraSystem.cpp"
  "./systems/EntityCollisionSystem.cpp"

  "./components/InputComponent.cpp"
  "./components/BoundingBoxComponent.cpp"
  "./components/ImpulseComponent.cpp"

  "./ui/UIManager.cpp"
  "./ui/UILayer.cpp"
  "./ui/UIElement.cpp"

  "./ui/elements/UIButton.cpp"
  "./ui/elements/UILabel.cpp"
)
add_library("${PROJECT_NAME}" "${SRC}")

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
