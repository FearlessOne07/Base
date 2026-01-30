set(SRC
  "./game/Game.cpp"

  "./entities/EntityManager.cpp"
  "./entities/Entity.cpp"

  "./timing/TimeManager.cpp"

  "./rendering/RenderContextSingleton.cpp"
  "./rendering/RenderingManager.cpp"
  "./rendering/Renderer.cpp"
  "./rendering/batchers/TextureBatcher.cpp"
  "./rendering/batchers/CircleBatcher.cpp"
  "./rendering/RenderLayer.cpp"
  "./rendering/Quad.cpp"
  "./rendering/Circlec.cpp"
  "./rendering/FrameBuffer.cpp"
  "./rendering/Material.cpp"
  "./rendering/ShaderEffect.cpp"
  "./rendering/Window.cpp"
  "./rendering/Vertex.cpp"
  "./rendering/VertexBuffer.cpp"
  "./rendering/IndexBuffer.cpp"
  "./rendering/VertexArray.cpp"
  "./rendering/RenderID.cpp"


  "./shaders/Shader.cpp"
  "./shaders/VertexShader.cpp"
  "./shaders/FragmentShader.cpp"

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

  "./camera/CameraController.cpp"
  "./camera/Camera.cpp"

  "./input/InputManager.cpp"

  "./signals/SignalBus.cpp"

  "./state/TransitionConditionBlock.cpp"

  "./assets/AssetManager.cpp"
  "./assets/BaseAsset.cpp"
  "./assets/Texture.cpp"
  "./assets/Font.cpp"

  "./tween/TweenManager.cpp"
  "./tween/Tween.cpp"

  "./util/Exception.cpp"
  "./util/Collisions.cpp"
  "./util/Easings.cpp"
  "./util/Strings.cpp"
  "./util/UUID.cpp"
  "./util/Pauseable.cpp"
  "./util/Math.cpp"
)

set(PV_HDRS
  "./include/internal/audio/SoundInstance.hpp"

  "./include/internal/input/InputManager.hpp"
  "./include/internal/input/InputListener.hpp"

  "./include/internal/scene/SceneManager.hpp"

  "./include/internal/utils/MSDFData.hpp"
  "./include/internal/utils/Collisions.hpp"
  "./include/internal/utils/TSCommandQueue.hpp"

  "./include/internal/camera/CameraManagerImpl.hpp"
  
  "./include/internal/systems/AreaSystem.hpp"
  "./include/internal/systems/MoveSystem.hpp"
  "./include/internal/systems/InputSystem.hpp"
  "./include/internal/systems/StateSystem.hpp"
  "./include/internal/systems/TimerSystem.hpp"
  "./include/internal/systems/RenderSystem.hpp"
  "./include/internal/systems/ProximitySystem.hpp" 
  "./include/internal/systems/EntityCollisionSystem.hpp"

  "./include/internal/particles/Particle.hpp"
  "./include/internal/particles/ParticleManagerImpl.hpp"

  "./include/internal/rendering/Vertex.hpp"
  "./include/internal/rendering/Window.hpp"
  "./include/internal/rendering/Renderer.hpp"
  "./include/internal/rendering/WindowMode.hpp"
  "./include/internal/rendering/IndexBuffer.hpp"
  "./include/internal/rendering/VertexArray.hpp"
  "./include/internal/rendering/VertexBuffer.hpp"
  "./include/internal/rendering/RenderCommand.hpp"
  "./include/internal/rendering/VertexBufferLayout.hpp" 
  "./include/internal/rendering/batchers/Batcher.hpp"
  "./include/internal/rendering/batchers/CircleBatcher.hpp"
  "./include/internal/rendering/batchers/TextureBatcher.hpp"
)

set(PB_HDRS
  # =======================
  # Assets
  # =======================
  "../include/base/assets/AssetHandle.hpp"
  "../include/base/assets/AssetManager.hpp"
  "../include/base/assets/BaseAsset.hpp"
  "../include/base/assets/Font.hpp"
  "../include/base/assets/Texture.hpp"

  # =======================
  # Audio
  # =======================
  "../include/base/audio/Sound.hpp"
  "../include/base/audio/AudioManager.hpp"
  "../include/base/audio/AudioStream.hpp"
  "../include/base/audio/signals/PlaySoundSignal.hpp"
  "../include/base/audio/signals/PlayAudioStreamSignal.hpp"
  "../include/base/audio/signals/StopAudioStreamSignal.hpp"

  # =======================
  # Camera
  # =======================
  "../include/base/camera/Camera.hpp"
  "../include/base/camera/CameraModes.hpp"
  "../include/base/camera/CameraController.hpp"
  "../include/base/camera/CameraShakeConfig.hpp"

  # =======================
  # Components
  # =======================
  "../include/base/components/AnimationComponent.hpp"
  "../include/base/components/AreaComponent.hpp"
  "../include/base/components/CircleComponent.hpp"
  "../include/base/components/ColliderComponent.hpp"
  "../include/base/components/Component.hpp"
  "../include/base/components/ComponentBundle.hpp"
  "../include/base/components/GravityComponent.hpp"
  "../include/base/components/ImpulseComponent.hpp"
  "../include/base/components/InputComponent.hpp"
  "../include/base/components/MoveComponent.hpp"
  "../include/base/components/ProximityComponent.hpp"
  "../include/base/components/QuadComponent.hpp"
  "../include/base/components/RigidBodyComponent.hpp"
  "../include/base/components/ShapeComponent.hpp"
  "../include/base/components/SpriteComponent.hpp"
  "../include/base/components/StateComponent.hpp"
  "../include/base/components/TimerComponent.hpp"
  "../include/base/components/TransformComponent.hpp"
  "../include/base/components/TransitionComponent.hpp"

  # =======================
  # Entities
  # =======================
  "../include/base/entities/Entity.hpp"
  "../include/base/entities/EntityManager.hpp"
  "../include/base/entities/signals/EntityCollisionSignal.hpp"

  # =======================
  # Game
  # =======================
  "../include/base/game/Game.hpp"
  "../include/base/game/GameConfig.hpp"
  "../include/base/game/GameContext.hpp"

  # =======================
  # Input
  # =======================
  "../include/base/input/Keys.hpp"
  "../include/base/input/InputEvent.hpp"
  "../include/base/input/MouseButtons.hpp"
  "../include/base/input/Events/KeyEvent.hpp"
  "../include/base/input/Events/MouseButtonEvent.hpp"

  # =======================
  # Particles
  # =======================
  "../include/base/particles/ParticleEmitter.hpp"
  "../include/base/particles/ParticleManager.hpp"

  # =======================
  # Rendering 
  # =======================
  "../include/base/rendering/Quad.hpp"
  "../include/base/rendering/Text.hpp"
  "../include/base/rendering/Origin.hpp"
  "../include/base/rendering/Sprite.hpp"
  "../include/base/rendering/Material.hpp"
  "../include/base/rendering/RenderID.hpp"
  "../include/base/rendering/Renderable.hpp"
  "../include/base/rendering/FrameBuffer.hpp"
  "../include/base/rendering/RenderLayer.hpp"
  "../include/base/rendering/GeometryType.hpp"
  "../include/base/rendering/RenderingManager.hpp"
  "../include/base/rendering/RenderContextSingleton.hpp"
  "../include/base/rendering/FramebufferAttachmentIndex.hpp"

  # =======================
  # Scenes
  # =======================
  "../include/base/scenes/Engine.hpp"
  "../include/base/scenes/Scene.hpp"
  "../include/base/scenes/SceneData.hpp"
  "../include/base/scenes/SceneID.hpp"
  "../include/base/scenes/SceneLayer.hpp"
  "../include/base/scenes/SceneLayerStack.hpp"
  "../include/base/scenes/SceneTransition.hpp"
  "../include/base/scenes/SharedSceneDataStore.hpp"
  "../include/base/scenes/signals/SceneLayerPausedSignal.hpp"
  "../include/base/scenes/signals/ScenePoppedSignal.hpp"
  "../include/base/scenes/signals/ScenePushedSignal.hpp"
  "../include/base/scenes/signals/SceneResumedSignal.hpp"
  "../include/base/scenes/signals/SceneSuspendedSignal.hpp"

  # =======================
  # Shaders
  # =======================
  "../include/base/shaders/FragmentShader.hpp"
  "../include/base/shaders/Shader.hpp"
  "../include/base/shaders/ShaderEffect.hpp"
  "../include/base/shaders/ShaderEffectChain.hpp"
  "../include/base/shaders/ShaderManager.hpp"
  "../include/base/shaders/UniformValue.hpp"
  "../include/base/shaders/VertexShader.hpp"

  # =======================
  # Signals
  # =======================
  "../include/base/signals/Signal.hpp"
  "../include/base/signals/SignalBus.hpp"

  # =======================
  # Sprites
  # =======================
  "../include/base/sprites/NinePatchSprite.hpp"
  "../include/base/sprites/Sprite.hpp"

  # =======================
  # State
  # =======================
  "../include/base/state/TransitionConditionBlock.hpp"

  # =======================
  # Systems
  # =======================
  "../include/base/systems/System.hpp"
  "../include/base/systems/SystemManager.hpp"

  # =======================
  # Textures
  # =======================
  "../include/base/textures/Font.hpp"
  "../include/base/textures/Texture.hpp"

  # =======================
  # Timing
  # =======================
  "../include/base/timing/TimeManager.hpp"

  # =======================
  # Tween
  # =======================
  "../include/base/tween/Tween.hpp"
  "../include/base/tween/TweenKey.hpp"
  "../include/base/tween/TweenManager.hpp"

  # =======================
  # UI
  # =======================
  "../include/base/ui/Binding.hpp"
  "../include/base/ui/UIConext.hpp"
  "../include/base/ui/UIElement.hpp"
  "../include/base/ui/UILayer.hpp"
  "../include/base/ui/UILayoutSettings.hpp"
  "../include/base/ui/UIManager.hpp"
  "../include/base/ui/elements/UIButton.hpp"
  "../include/base/ui/elements/UICanvas.hpp"
  "../include/base/ui/elements/UIFlexContainer.hpp"
  "../include/base/ui/elements/UIGrid.hpp"
  "../include/base/ui/elements/UILabel.hpp"
  "../include/base/ui/elements/UIPanel.hpp"
  "../include/base/ui/elements/UIStackPanel.hpp"
  "../include/base/ui/elements/UITextureRect.hpp"

  # =======================
  # Util
  # =======================
  "../include/base/util/AntagonisticFunction.hpp"
  "../include/base/util/Circle.hpp"
  "../include/base/util/Colors.hpp"
  "../include/base/util/Draw.hpp"
  "../include/base/util/Easings.hpp"
  "../include/base/util/Exception.hpp"
  "../include/base/util/FastNoiseLite.hpp"
  "../include/base/util/Math.hpp"
  "../include/base/util/Pauseable.hpp"
  "../include/base/util/QuadTree.hpp"
  "../include/base/util/QuadTreeContainer.hpp"
  "../include/base/util/Ref.hpp"
  "../include/base/util/Strings.hpp"
  "../include/base/util/Type.hpp"
  "../include/base/util/UUID.hpp"
)


add_library("${PROJECT_NAME}"
    "${SRC}"
    "${PV_HDRS}"
    "${PB_HDRS}"
)

target_link_libraries("${PROJECT_NAME}"
  PRIVATE
    msdf-atlas-gen
    portaudio
    miniaudio
    samplerate
    glad
    glfw
    glm
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
    "${ROOT_BASE_DIR}/src/include/"
    "${ROOT_BASE_DIR}/vendor/msdf-atlas-gen/"
    "${ROOT_BASE_DIR}/vendor/msdf-atlas-gen/msdfgen/"
    "${ROOT_BASE_DIR}/vendor/stb_image/"
    "${ROOT_BASE_DIR}/include/"
)
