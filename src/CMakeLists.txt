set(SRC
  "./game/Game.cpp"

  "./entities/EntityManager.cpp"
  "./entities/Entity.cpp"

  "./rendering/RenderContextSingleton.cpp"
  "./rendering/RenderingManager.cpp"
  "./rendering/RenderLayer.cpp"

  "./sprites/Sprite.cpp"
  "./sprites/NinePatchSprite.cpp"

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
  "./ui/elements/UIStackPanel.cpp"
  "./ui/elements/UITextureRect.cpp"
  "./ui/elements/UIFlexContainer.cpp"
  "./ui/elements/UICanvas.cpp"
  "./ui/elements/UIGrid.cpp"

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
  "./systems/ProximitySystem.cpp"
  "./systems/StateSystem.cpp"
  "./systems/AreaSystem.cpp"
  "./systems/TimerSystem.cpp"

  "./components/Component.cpp"
  "./components/CircleComponent.cpp"
  "./components/InputComponent.cpp"
  "./components/ColliderComponent.cpp"
  "./components/ImpulseComponent.cpp"
  "./components/AnimationComponent.cpp"
  "./components/SpriteComponent.cpp"
  "./components/ComponentBundle.cpp"
  "./components/StateComponent.cpp"
  "./components/ProximityComponent.cpp"
  "./components/AreaComponent.cpp"
  "./components/TransitionComponent.cpp"
  "./components/QuadComponent.cpp"
  "./components/TimerComponent.cpp"

  "./particles/ParticleManager.cpp"

  "./camera/Camera2DExt.cpp"

  "./input/InputManager.cpp"

  "./signals/SignalBus.cpp"

  "./state/TransitionConditionBlock.cpp"

  "./assets/AssetManager.cpp"
  "./assets/BaseAsset.cpp"

  "./tween/TweenManager.cpp"
  "./tween/Tween.cpp"

  "./util/Exception.cpp"
  "./util/Easings.cpp"
  "./util/Strings.cpp"
  "./util/UUID.cpp"
  "./util/Pauseable.cpp"
  "./util/Draw.cpp"

  "./shaders/ShaderManager.cpp"
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

if(WIN32)
  target_compile_definitions("${PROJECT_NAME}"
    PRIVATE
      WIN32_LEAN_AND_MEAN
      NOMINMAX
      NOGDI
      NOUSER
  )
endif()

target_include_directories("${PROJECT_NAME}"
  PUBLIC
    "${ROOT_BASE_DIR}/vendor/miniaudio/"
    "${ROOT_BASE_DIR}/vendor/libsamplerate/include"
    "${ROOT_BASE_DIR}/vendor/glm/"
    "${ROOT_BASE_DIR}/vendor/glfw/include/"
    "${ROOT_BASE_DIR}/vendor/glad/include/"
    "${ROOT_BASE_DIR}/vendor/msdf-atlas-gen/"
    "${ROOT_BASE_DIR}/src/include/"
  PRIVATE
    "${ROOT_BASE_DIR}/vendor/stb_image/"
    "${ROOT_BASE_DIR}/include/"
)
