#include "base/rendering/FrameBuffer.hpp"
#include "base/util/Exception.hpp"
#include "glad/glad.h"
#include <iostream>
#include <memory>
#include <vector>

namespace Base
{
  std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpec &spec)
  {
    return std::shared_ptr<FrameBuffer>(new FrameBuffer(spec));
  }

  void FrameBuffer::Destroy(std::shared_ptr<FrameBuffer> frameBuffer)
  {
    for (int i = 0; i < frameBuffer->_colorAttachmentCount; i++)
    {
      Texture::Destroy(frameBuffer->_colorAttachments[i]);
    }
    glDeleteFramebuffers(1, frameBuffer->_id.data());
  }

  FrameBuffer::FrameBuffer(const FrameBufferSpec &spec) : _width(spec.Width), _height(spec.Height)
  {
    glCreateFramebuffers(1, _id.data());
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    if (spec.ColorAttachmentCount > MAX_COLOR_ATTACHMENTS)
    {
      THROW_BASE_RUNTIME_ERROR("Invalide Number of Color Attachments for Framebuffer; Max is 8");
    }

    _colorAttachmentCount = spec.ColorAttachmentCount;
    for (int i = 0; i < spec.ColorAttachmentCount; i++)
    {
      _colorAttachments[i] = Texture::Create( //
        nullptr,
        TextureSpec{
          .Width = _width,
          .Height = _height,
          .Format = spec.ColorAttachmentFormats[i],
          .MipMaps = false,
        } //
      );
      glFramebufferTexture2D( //
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorAttachments[i]->GetRenderID(),
        0 //
      );
    }

    glGenRenderbuffers(1, _depthAttachment.data());
    glBindRenderbuffer(GL_RENDERBUFFER, _depthAttachment);
    glRenderbufferStorage( //
      GL_RENDERBUFFER,
      GL_DEPTH24_STENCIL8,    // or GL_DEPTH_COMPONENT24
      spec.Width, spec.Height //
    );

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthAttachment);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void FrameBuffer::Bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    std::vector<GLenum> bufs;
    for (int i = 0; i < _colorAttachmentCount; i++)
    {
      bufs.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(_colorAttachmentCount, bufs.data());
  }

  void FrameBuffer::Unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
  }

  std::shared_ptr<Texture> FrameBuffer::GetColorAttachment(FramebufferAttachmentIndex attachmentIndex) const
  {
    if (!(static_cast<uint8_t>(attachmentIndex) < _colorAttachmentCount))
    {
      THROW_BASE_RUNTIME_ERROR("Attachement Index out of bounds!");
    }

    return _colorAttachments[static_cast<uint8_t>(attachmentIndex)];
  }

  int FrameBuffer::GetWidth() const
  {
    return _width;
  }

  int FrameBuffer::GetHeight() const
  {
    return _height;
  }

  RenderID FrameBuffer::GetRenderId() const
  {
    return _id;
  }
} // namespace Base
