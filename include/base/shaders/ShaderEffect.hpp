#pragma once
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "base/rendering/Material.hpp"
#include "base/util/Type.hpp"
#include <memory>
#include <unordered_set>

namespace Base
{
  class Scene;
  class FrameBuffer;
  class ShaderEffect
  {
  protected:
    bool _active = true;

    void BeginFrameBuffer(Ptr<FrameBuffer> framebuffer);
    void DrawFrameBuffer( //
      std::shared_ptr<FrameBuffer> framebuffer, Vector2 size, const Material &material,
      FramebufferAttachmentIndex attachmentToDraw,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments = {FramebufferAttachmentIndex::Color0} //
    );
    void ClearFrameBuffer( //
      Color color,
      const FramebufferAttachmentIndex &attachmentToClear = FramebufferAttachmentIndex::Color0 //
    );
    void EndFrameBuffer();

  public:
    virtual ~ShaderEffect() = default;
    virtual void Apply(Ptr<FrameBuffer> input, Ptr<FrameBuffer> output, Vector2 resolution) = 0;
    virtual void Update(float dt) {};

    bool IsAvtive() const
    {
      return _active;
    }
  };
} // namespace Base
