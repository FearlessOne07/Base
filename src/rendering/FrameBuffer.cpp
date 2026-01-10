#include "FrameBuffer.hpp"
#include "glad/glad.h"
#include "texture/Texture.hpp"
#include <iostream>
#include <memory>

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

  for (int i = 0; i < spec.ColorAttachments.size(); i++)
  {
    _colorAttachments[i] = Texture::Create(
      nullptr, TextureSpec{.Width = _width, .Height = _height, .Format = spec.ColorAttachments[i], .MipMaps = false});
    glFramebufferTexture2D( //
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorAttachments[i]->GetRenderID(),
      0 //
    );
    _colorAttachmentCount++;
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
  static constexpr GLenum bufs[8] = {
    GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7,
  };
  glDrawBuffers(8, bufs);
}

void FrameBuffer::Unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK);
}

std::shared_ptr<Texture> FrameBuffer::GetColorAttachment(FramebufferAttachmentIndex attachmentIndex) const
{
  if (!(static_cast<uint8_t>(attachmentIndex) + 1 <= _colorAttachmentCount))
  {
    std::cout << "Attachement Index out of bounds!\n";
    exit(-1);
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

} // namespace Base
