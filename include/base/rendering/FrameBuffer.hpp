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
  enum class FrameBufferFormat
  {
  };

  struct FrameBufferSpec
  {
    float Width = 0;
    float Height = 0;
    std::array<TextureFormat, 8> ColorAttachments = {
      TextureFormat::RGBA8, TextureFormat::RGBA8, TextureFormat::RGBA8, TextureFormat::RGBA8,
      TextureFormat::RGBA8, TextureFormat::RGBA8, TextureFormat::RGBA8, TextureFormat::RGBA8,
    };
  };

  class FrameBuffer
  {
  private:
    friend class Renderer;
    RenderID _id;
    RenderID _depthAttachment;

    std::array<std::shared_ptr<Texture>, 8> _colorAttachments;
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
