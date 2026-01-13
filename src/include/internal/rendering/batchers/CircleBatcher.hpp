#pragma once

#include "Batcher.hpp"
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "internal/rendering/Vertex.hpp"
#include <unordered_set>

namespace Base
{

  class CircleBatcher : public Batcher
  {
  private:
    static constexpr uint32_t _maxQuads = 1000;
    static constexpr uint32_t _maxVertices = _maxQuads * 4;
    static constexpr uint32_t _maxIndices = _maxQuads * 6;

    std::vector<CircleVertex> _vertices;

  private:
    void DrawCircle( //
      const Circle &circle, Vector3 position, glm::vec4 color, float lineThickness,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments //
    );

  public:
    void Init() override;
    void DeInit() override;
    void Begin() override;
    void Flush() override;
    void Submit(const RenderCommand &command) override;
  };

} // namespace Base
