set(SRC
  "./game/Game.cpp"
  "./game/RenderContextSingleton.cpp"

  "./entities/EntityManager.cpp"
  "./entities/Entity.cpp"

  "./audio/Sound.cpp"
  "./audio/AudioManager.cpp"

  "./ui/UILayer.cpp"
  "./ui/UIManager.cpp"
  "./ui/UIElement.cpp"
  "./ui/elements/UILabel.cpp"
  "./ui/elements/UIButton.cpp"

  "./scenes/SceneManager.cpp"
  "./scenes/Scene.cpp"
  "./scenes/SceneLayerStack.cpp"
  "./scenes/SceneLayer.cpp"

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

  "./signals/SignalBus.cpp"

  "./assets/AssetManager.cpp"

  "./tween/TweenManager.cpp"
  "./tween/ITween.cpp"

  "./util/Exception.cpp"
  "./util/Easings.cpp"
  "./util/Strings.cpp"
)
add_library("${PROJECT_NAME}" "${SRC}")

target_link_libraries("${PROJECT_NAME}"
  PUBLIC
    raylib
  PRIVATE
    portaudio
)

target_link_directories("${PROJECT_NAME}"
  PRIVATE
    "${ROOT_BASE_DIR}/build/vendor/raylib"
    "${ROOT_BASE_DIR}/build/vendor/portaudio"
)

target_compile_definitions("${PROJECT_NAME}"
  PRIVATE
    MINIAUDIO_IMPLEMENTATION
)
target_include_directories("${PROJECT_NAME}"
  PRIVATE
    "${ROOT_BASE_DIR}/src/include/"
    "${ROOT_BASE_DIR}/include/"
    "${ROOT_BASE_DIR}/vendor/miniaudio/"
)
