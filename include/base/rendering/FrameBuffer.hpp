#pragma once
#include "base/assets/Texture.hpp"
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "base/rendering/RenderID.hpp"
#include "glad/glad.h"
#include <array>
#include <cstdint>
#include <memory>

namespace Base
{
  struct FrameBufferSpec
  {
    int Width = 0;
    int Height = 0;
    uint8_t ColorAttachmentCount = 1;
    std::array<TextureFormat, 8> ColorAttachmentFormats = {
      TextureFormat::RGBA8, TextureFormat::RGBA8, TextureFormat::RGBA8, TextureFormat::RGBA8,
      TextureFormat::RGBA8, TextureFormat::RGBA8, TextureFormat::RGBA8, TextureFormat::RGBA8,
    };
  };

  class FrameBuffer
  {
  private:
    static constexpr uint8_t MAX_COLOR_ATTACHMENTS = 8;
    friend class Renderer;
    RenderID _id;
    RenderID _depthAttachment;

    std::array<std::shared_ptr<Texture>, MAX_COLOR_ATTACHMENTS> _colorAttachments;
    uint8_t _colorAttachmentCount = 0;

    int _width = 0, _height = 0;

  private:
    FrameBuffer(const FrameBufferSpec &spec);

    void Bind();
    void Unbind();

  public:
    static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpec &spec);
    static void Destroy(std::shared_ptr<FrameBuffer> frameBuffer);
    std::shared_ptr<Texture> GetColorAttachment(FramebufferAttachmentIndex attachmentIndex) const;

    int GetWidth() const;
    int GetHeight() const;
    RenderID GetRenderId() const;
  };
} // namespace Base
