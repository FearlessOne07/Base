set(SRC
  "./game/Game.cpp"
  "./game/RenderContextSingleton.cpp"

  "./entities/EntityManager.cpp"
  "./entities/Entity.cpp"

  "./ui/UILayer.cpp"
  "./ui/UIManager.cpp"
  "./ui/UIElement.cpp"

  "./scenes/SceneManager.cpp"
  "./scenes/Scene.cpp"
  "./scenes/SceneLayerStack.cpp"

  "./systems/SystemManager.cpp"
  "./systems/System.cpp"
  "./systems/MoveSystem.cpp"
  "./systems/RenderSystem.cpp"
  "./systems/InputSystem.cpp"
  "./systems/EntityCollisionSystem.cpp"

  "./components/Component.cpp"
  "./components/InputComponent.cpp"
  "./components/ColliderComponent.cpp"
  "./components/ImpulseComponent.cpp"

  "./particles/ParticleManager.cpp"

  "./camera/CameraManager.cpp"

  "./input/InputManager.cpp"

  "./signals/SignalManager.cpp"

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
