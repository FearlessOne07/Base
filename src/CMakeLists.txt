set(SRC
  "./game/Game.cpp"

  "./entities/EntityManager.cpp"
  "./entities/Entity.cpp"

  "./renderer/RenderContextSingleton.cpp"
  "./renderer/Renderer.cpp"
  "./renderer/RenderLayer.cpp"

  "./audio/Sound.cpp"
  "./audio/AudioStream.cpp"
  "./audio/AudioManager.cpp"
  "./audio/SoundInstance.cpp"

  "./ui/UILayer.cpp"
  "./ui/UIManager.cpp"
  "./ui/UIElement.cpp"
  "./ui/elements/UILabel.cpp"
  "./ui/elements/UIButton.cpp"
  "./ui/elements/UIPanel.cpp"
  "./ui/elements/UIContainer.cpp"

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

  "./camera/Camera2DExt.cpp"

  "./input/InputManager.cpp"

  "./signals/SignalBus.cpp"

  "./assets/AssetManager.cpp"
  "./assets/BaseAsset.cpp"

  "./tween/TweenManager.cpp"
  "./tween/ITween.cpp"

  "./util/Exception.cpp"
  "./util/Easings.cpp"
  "./util/Strings.cpp"
  "./util/UUID.cpp"

  "./shaders/ShaderManager.cpp"
  "./shaders/ShaderPass.cpp"
  "./shaders/ShaderChain.cpp"
)
add_library("${PROJECT_NAME}" "${SRC}")

target_link_libraries("${PROJECT_NAME}"
  PUBLIC
    raylib
  PRIVATE
    portaudio
    miniaudio
    samplerate
)

target_link_directories("${PROJECT_NAME}"
  PRIVATE
    "${ROOT_BASE_DIR}/build/vendor/raylib"
    "${ROOT_BASE_DIR}/build/vendor/portaudio"
    "${ROOT_BASE_DIR}/build/vendor/libsamplerate"
)

target_include_directories("${PROJECT_NAME}"
  PUBLIC
    "${ROOT_BASE_DIR}/vendor/miniaudio/"
    "${ROOT_BASE_DIR}/vendor/libsamplerate/include"
  PRIVATE
    "${ROOT_BASE_DIR}/src/include/"
    "${ROOT_BASE_DIR}/include/"
)
