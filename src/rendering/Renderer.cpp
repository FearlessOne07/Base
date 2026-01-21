#include "internal/rendering/Renderer.hpp"
#include "base/util/Math.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Base
{
  void Renderer::Init(const RenderSpec &spec)
  {
    if (!_instance)
    {
      _instance = std::make_unique<Renderer>();
      _instance->_init(spec);
    }
  }

  void Renderer::_init(const RenderSpec &spec)
  {
    // Create Window
    _window = std::make_shared<Window>();
    _window->CreateWindow({
      .Title = spec.Title,
      .MinSize = spec.MinWindowSize,
      .Vsync = spec.Vysnc,
    });

    Shader::SetMaxTextureSlots(_window->GetGlContexData().MaxTextureUnits);
    _defaultCamera = std::make_shared<Camera>(glm::vec2{_window->GetWindowWidth(), _window->GetWindowHeight()});
    _currentCamera = _defaultCamera;

    _quadBatcher.Init();
    _quadBatcher.SetCamera(_defaultCamera);
    _quadBatcher.SetMaxTextureSlots(_window->GetGlContexData().MaxTextureUnits);

    _textBatcher.Init();
    _textBatcher.SetCamera(_defaultCamera);

    _circleBatcher.Init();
    _circleBatcher.SetCamera(_defaultCamera);
  }

  void Renderer::DrawQuad( //
    const Rectangle &quad, glm::vec2 position, Color color, float rotationDeg,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    Vector4 colorF = Vector4(color) / 255.f;
    colorF = Math::Clamp(colorF, 0.f, 1.f);

    _instance->_renderQueue.push(
      QuadCommand{quad, glm::vec3(position, _currentZIndex), colorF, rotationDeg, attachments});
    _currentZIndex += _Zstep;
  }

  void Renderer::DrawQuad( //
    const Rectangle &quad, glm::vec2 position, Color color,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    DrawQuad(quad, position, color, 0.f, attachments);
  }

  void Renderer::DrawSprite( //
    const Sprite &sprite, glm::vec2 position, glm::vec2 size, float rotationDeg,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    _instance->_renderQueue.push(
      SpriteCommand{sprite, glm::vec3(position, _currentZIndex), size, rotationDeg, attachments} //
    );
    _currentZIndex += _Zstep;
  }

  void Renderer::DrawSprite( //
    const Sprite &sprite, glm::vec2 position, glm::vec2 size,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    DrawSprite(sprite, position, size, 0.f, attachments);
  }

  void Renderer::DrawFramebuffer( //
    std::shared_ptr<FrameBuffer> framebuffer, glm::vec2 size, FramebufferAttachmentIndex attachmentToDraw,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    DrawSprite(                                                                           //
      {framebuffer->GetColorAttachment(attachmentToDraw)}, {0, 0}, size, 0.f, attachments //
    );
  }

  void Renderer::DrawFramebuffer( //
    std::shared_ptr<FrameBuffer> framebuffer, glm::vec2 position, glm::vec2 size,
    FramebufferAttachmentIndex attachmentToDraw,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    DrawSprite(                                                                             //
      {framebuffer->GetColorAttachment(attachmentToDraw)}, position, size, 0.f, attachments //
    );
  }

  void Renderer::DrawFramebuffer( //
    std::shared_ptr<FrameBuffer> framebuffer, glm::vec2 position, glm::vec2 size, const Material &material,
    FramebufferAttachmentIndex attachmentToDraw,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    DrawSprite(                                                                                       //
      {framebuffer->GetColorAttachment(attachmentToDraw), material}, position, size, 0.f, attachments //
    );
  }

  void Renderer::DrawFramebuffer( //
    std::shared_ptr<FrameBuffer> framebuffer, glm::vec2 size, const Material &material,
    FramebufferAttachmentIndex attachmentToDraw,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    DrawSprite(                                                                                           //
      Sprite(framebuffer->GetColorAttachment(attachmentToDraw), material), {0, 0}, size, 0.f, attachments //
    );
  }

  void Renderer::DrawText(                                                                                      //
    const std::string &text, glm::vec2 position, Color color, float fontSize, const std::shared_ptr<Font> font, //
    const std::unordered_set<FramebufferAttachmentIndex> &attachments                                           //
  )
  {
    Vector4 colorF = Vector4(color) / 255.f;
    colorF = Math::Clamp(colorF, 0.f, 1.f);
    _instance->_renderQueue.push(
      TextCommand{text, font, glm::vec3(position, _currentZIndex), colorF, fontSize, attachments} //
    );
    _currentZIndex += _Zstep;
  }

  void Renderer::DrawCircle( //
    const Circle &circle, Vector2 position, Color color,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    DrawCircle(circle, position, color, circle.GetRadius(), attachments);
  }

  void Renderer::DrawCircle( //
    const Circle &circle, Vector2 position, Color color, float thickness,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    Vector4 colorF = Vector4(color) / 255.f;
    colorF = Math::Clamp(colorF, 0.f, 1.f);

    _instance->_renderQueue.push(                                                              //
      CircleCommand{circle, Vector3(position, _currentZIndex), colorF, thickness, attachments} //
    );
    _currentZIndex += _Zstep;
  }

  void Renderer::Update()
  {
    _instance->_windowViewPort = {
      _instance->_window->GetFrameBufferWidth(),
      _instance->_window->GetFrameBufferHeight(),
    };
    _instance->_defaultCamera->SetViewPort(_instance->_windowViewPort);
    glViewport(0, 0, _instance->_window->GetFrameBufferWidth(), _instance->_window->GetFrameBufferHeight());
  }

  void Renderer::Clear(Color color, FramebufferAttachmentIndex attachment)
  {
    Vector4 colorF = Vector4(color) / 255.f;
    colorF = Math::Clamp(colorF, 0.f, 1.f);

    _instance->_renderQueue.push(ClearCommand{colorF, attachment});
  }

  void Renderer::BeginFrame()
  {
    _currentZIndex = 0;
  }

  void Renderer::EndFrame()
  {
    _instance->ExecuteRenderCommands();
  }

  void Renderer::BeginCamera(std::shared_ptr<Camera> camera)
  {
    _instance->_renderQueue.push(SetCameraCommand{camera});
  }

  void Renderer::EndCamera()
  {
    _instance->_renderQueue.push(SetCameraCommand{_instance->_defaultCamera});
  }

  void Renderer::BeginFramebuffer(std::shared_ptr<FrameBuffer> framebuffer)
  {
    _instance->_renderQueue.push(SetFramebufferCommand{framebuffer});
  }

  void Renderer::EndFramebuffer()
  {
    _instance->_renderQueue.push(SetFramebufferCommand{nullptr});
  }

  void Renderer::PollWindow()
  {
    _instance->_window->Poll();
  }

  void Renderer::SetWindowMode(const WindowMode &mode)
  {
    _instance->_window->SetMode(mode);
  }

  void Renderer::SwapWindowBuffers()
  {
    _instance->_window->SwapBuffers();
  }

  bool Renderer::IsWindowClosed()
  {
    return _instance->_window->WindowClosed();
  }
  IVector2 Renderer::GetWindowSize()
  {
    return {_instance->_window->GetWindowWidth(), _instance->_window->GetWindowHeight()};
  };

  IVector2 Renderer::GetFramebufferSize()
  {
    return {_instance->_window->GetFrameBufferWidth(), _instance->_window->GetFrameBufferHeight()};
  }

  Vector2 Renderer::GetWindowMousePosition()
  {
    return _instance->_window->GetMousePosition();
  }

  bool Renderer::GetWindowKeyState(const Key &key)
  {
    return _instance->_window->GetKeyState(key);
  }

  bool Renderer::GetWindowMouseButtonState(const MouseKey &mouseKey)
  {
    return _instance->_window->GetMouseButtonState(mouseKey);
  }

  void Renderer::SetScrollCallback(const ScrollCallback &scrollCallback)
  {
    _instance->_window->SetScrollCallback(scrollCallback);
  }

  void Renderer::SetCharCallback(const CharCallback &charCallback)
  {
    _instance->_window->SetCharCallback(charCallback);
  }
  void Renderer::Deinit()
  {
    _instance->_quadBatcher.DeInit();
    _instance->_textBatcher.DeInit();
    _instance->_circleBatcher.DeInit();
    _instance->_window->Close();
  }

  void Renderer::ExecuteRenderCommands()
  {
    while (!_renderQueue.empty())
    {
      auto command = _renderQueue.front();
      _renderQueue.pop();

      if (std::holds_alternative<ClearCommand>(command))
      {
        auto com = std::get<ClearCommand>(command);
        glm::vec4 color = com.Color;

        if (_instance->_currentFramebuffer)
        {
          glClearBufferfv(GL_COLOR, static_cast<uint8_t>(com.AttachmentToClear), glm::value_ptr(color));
        }
        else
        {
          glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
        }
      }
      else if (std::holds_alternative<SetFramebufferCommand>(command))
      {
        auto com = std::get<SetFramebufferCommand>(command);
        auto &framebuffer = com.FramebufferToSet;

        if (_currentBatcher)
        {
          _currentBatcher->Flush();
        }

        if (framebuffer)
        {
          _instance->_currentFramebuffer = framebuffer;
          framebuffer->Bind();
          _instance->_defaultCamera->SetViewPort({framebuffer->GetWidth(), framebuffer->GetHeight()});
          glViewport(0, 0, framebuffer->GetWidth(), framebuffer->GetHeight());
        }
        else
        {
          glBindFramebuffer(GL_FRAMEBUFFER, 0);
          _instance->_currentFramebuffer = nullptr;
          // Restore window viewport
          _instance->_defaultCamera->SetViewPort({_instance->_windowViewPort.x, _instance->_windowViewPort.y});
          glViewport(0, 0, _instance->_windowViewPort.x, _instance->_windowViewPort.y);
        }

        if (_currentBatcher)
        {
          _currentBatcher->Begin();
        }
      }
      else if (std::holds_alternative<SetCameraCommand>(command))
      {
        if (_currentBatcher)
        {
          _currentBatcher->Flush();
        }
        auto com = std::get<SetCameraCommand>(command);
        _currentCamera = com.CameraToSet;

        _quadBatcher.SetCamera(_currentCamera);
        _textBatcher.SetCamera(_currentCamera);
        _circleBatcher.SetCamera(_currentCamera);
        if (_currentBatcher)
        {
          _currentBatcher->Begin();
        }
      }
      else if (std::holds_alternative<QuadCommand>(command))
      {
        if (!_currentBatcher)
        {
          _currentBatcher = &_quadBatcher;
          _currentBatcher->Begin();
        }
        else if (_currentBatcher->GetGeometryType() != GeometryType::Quad)
        {
          _currentBatcher->Flush();
          _currentBatcher = &_quadBatcher;
          _currentBatcher->Begin();
        }
        _currentBatcher->Submit(command);
      }
      else if (std::holds_alternative<SpriteCommand>(command))
      {
        if (!_currentBatcher)
        {
          _currentBatcher = &_quadBatcher;
        }
        else if (_currentBatcher->GetGeometryType() != GeometryType::Quad)
        {
          _currentBatcher->Flush();
          _currentBatcher = &_quadBatcher;
          _currentBatcher->Begin();
        }
        _currentBatcher->Submit(command);
      }
      else if (std::holds_alternative<CircleCommand>(command))
      {
        if (!_currentBatcher)
        {
          _currentBatcher = &_circleBatcher;
        }
        else if (_currentBatcher->GetGeometryType() != GeometryType::Circle)
        {
          _currentBatcher->Flush();
          _currentBatcher = &_circleBatcher;
          _currentBatcher->Begin();
        }
        _currentBatcher->Submit(command);
      }
      else if (std::holds_alternative<TextCommand>(command))
      {
        if (!_currentBatcher)
        {
          _currentBatcher = &_textBatcher;
        }
        else if (_currentBatcher->GetGeometryType() != GeometryType::Text)
        {
          _currentBatcher->Flush();
          _currentBatcher = &_textBatcher;
          _currentBatcher->Begin();
        }
        _currentBatcher->Submit(command);
      }
    }

    if (_currentBatcher)
    {
      _currentBatcher->Flush();
    }
    _currentBatcher = nullptr;
  }
} // namespace Base
