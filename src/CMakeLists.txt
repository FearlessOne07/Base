set(SRC
  "./game/Game.cpp"
  "./game/RenderContextSingleton.cpp"
  "./game/RenderContext.cpp"

  "./entities/EntityManager.cpp"
  "./entities/Entity.cpp"

  "./scenes/SceneManager.cpp"
  "./scenes/Scene.cpp"

  "./systems/SystemManager.cpp"
  "./systems/System.cpp"
  "./systems/MoveSystem.cpp"
  "./systems/RenderSystem.cpp"
  "./systems/InputSystem.cpp"
  "./systems/CameraSystem.cpp"
  "./systems/EntityCollisionSystem.cpp"

  "./components/Component.cpp"
  "./components/InputComponent.cpp"
  "./components/ColliderComponent.cpp"
  "./components/ImpulseComponent.cpp"

  "./ui/UIManager.cpp"
  "./ui/UILayer.cpp"
  "./ui/UIElement.cpp"

  "./ui/elements/UIButton.cpp"
  "./ui/elements/UILabel.cpp"

  "./particles/ParticleManager.cpp"

  "./input/InputManager.cpp"

  "./signals//EventBus.cpp"

  "./assets/AssetManager.cpp"

  "./util/Exception.cpp"
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
