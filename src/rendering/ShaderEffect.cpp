#include "base/shaders/ShaderEffect.hpp"
#include "internal/rendering/Renderer.hpp"

namespace Base
{
  void ShaderEffect::BeginFrameBuffer(Ptr<FrameBuffer> framebuffer)
  {
    Renderer::BeginFramebuffer(framebuffer);
  }

  void ShaderEffect::DrawFrameBuffer( //
    std::shared_ptr<FrameBuffer> framebuffer, Vector2 size, const Material &material,
    FramebufferAttachmentIndex attachmentToDraw,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    Renderer::DrawFramebuffer(framebuffer, {0, 0}, size, material, attachmentToDraw, attachments);
  }

  void ShaderEffect::ClearFrameBuffer(Color color, const FramebufferAttachmentIndex &attachmentToClear)
  {
    Renderer::Clear(color, attachmentToClear);
  }

  void ShaderEffect::EndFrameBuffer()
  {
    Renderer::EndFramebuffer();
  }

} // namespace Base
