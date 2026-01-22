#pragma once
#include "base/assets/Font.hpp"
#include "base/camera/Camera.hpp"
#include "base/input/Keys.hpp"
#include "base/rendering/FrameBuffer.hpp"
#include "base/rendering/Sprite.hpp"
#include "base/util/Circle.hpp"
#include "base/util/Type.hpp"
#include "internal/rendering/Window.hpp"
#include "internal/rendering/WindowMode.hpp"
#include "internal/rendering/batchers/CircleBatcher.hpp"
#include "internal/rendering/batchers/QuadBatcher.hpp"
#include "internal/rendering/batchers/TextBatcher.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <queue>
#include <unordered_set>

namespace Base
{

  class Window;

  struct RenderSpec
  {
    const char *Title = "Window";
    IVector2 MinWindowSize = {1280, 720};
    bool Vysnc = false;
    bool ResizableWindow = true;
  };

  class Renderer
  {
  private:
    // Instance
    inline static std::shared_ptr<Renderer> _instance = nullptr;
    inline static float _Zstep = 1e-4f;
    inline static float _currentZIndex = 0;

    // Window
    std::shared_ptr<Window> _window = nullptr;
    glm::vec2 _windowViewPort = {0, 0};

    // Batchers
    QuadBatcher _quadBatcher;
    TextBatcher _textBatcher;
    CircleBatcher _circleBatcher;

    // Currents
    std::shared_ptr<Camera> _currentCamera = nullptr;
    std::shared_ptr<FrameBuffer> _currentFramebuffer = nullptr;
    Batcher *_currentBatcher = nullptr;

    // Defaults
    std::shared_ptr<Camera> _defaultCamera = nullptr;

    // Render Queue
    std::queue<RenderCommand> _renderQueue;

  private:
    void _init(const RenderSpec &spec);
    void _shutdown();
    void ExecuteRenderCommands();

  public:
    static void Init(const RenderSpec &spec = {});
    static void Deinit();
    static void Shutdown();
    static void Update();

    // Window
    static void PollWindow();
    static void SwapWindowBuffers();
    static bool IsWindowClosed();
    static void SetScrollCallback(const ScrollCallback &scrollCallback);
    static void SetCharCallback(const CharCallback &charCallback);
    static void SetWindowMode(const WindowMode &state);
    static IVector2 GetWindowSize();
    static IVector2 GetFramebufferSize();
    static Vector2 GetWindowMousePosition();
    static bool GetWindowKeyState(const Key &key);
    static bool GetWindowMouseButtonState(const MouseKey &mouseKey);

    // Draw
    static void DrawQuad( //
      const Rectangle &quad, glm::vec2 position, Color color,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawQuad( //
      const Rectangle &quad, glm::vec2 position, Color color, float rotationDeg,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawSprite( //
      const Sprite &sprite, glm::vec2 position, glm::vec2 size,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawSprite( //
      const Sprite &sprite, glm::vec2 position, glm::vec2 size, float rotationDeg, Color color,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawFramebuffer( //
      std::shared_ptr<FrameBuffer> framebuffer, glm::vec2 size,
      FramebufferAttachmentIndex attachmentToDraw,                                                             //
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawFramebuffer( //
      std::shared_ptr<FrameBuffer> framebuffer, glm::vec2 size, const Material &material,
      FramebufferAttachmentIndex attachmentToDraw,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawFramebuffer( //
      std::shared_ptr<FrameBuffer> framebuffer, glm::vec2 position, glm::vec2 size,
      FramebufferAttachmentIndex attachmentToDraw,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );

    static void DrawFramebuffer( //
      std::shared_ptr<FrameBuffer> framebuffer, glm::vec2 position, glm::vec2 size, const Material &material,
      FramebufferAttachmentIndex attachmentToDraw,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawText( //
      const std::string &text, glm::vec2 position, Color color, float fontSize,
      const std::shared_ptr<Font> font,                                                                        //
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawCircle( //
      const Circle &circle, Vector2 position, Color color,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void DrawCircle( //
      const Circle &circle, Vector2 position, Color color, float thickness,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    static void Clear(Color color, FramebufferAttachmentIndex attachments = FramebufferAttachmentIndex::Color0);

    // Render State
    static void BeginFrame();
    static void EndFrame();

    static void BeginCamera(std::shared_ptr<Camera> camera);
    static void EndCamera();

    static void BeginFramebuffer(std::shared_ptr<FrameBuffer> framebuffer);
    static void EndFramebuffer();
  };

} // namespace Base
