#pragma once
#include "Batcher.hpp"
#include "base/assets/Font.hpp"
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "base/shaders/Shader.hpp"
#include "glm/fwd.hpp"
#include "internal/rendering/Vertex.hpp"
#include <cstdint>
#include <memory>
#include <unordered_set>

namespace Base
{

  class TextBatcher : public Batcher
  {
  private:
    static constexpr uint32_t _maxQuads = 1000;
    static constexpr uint32_t _maxVertices = _maxQuads * 4;
    static constexpr uint32_t _maxIndices = _maxQuads * 6;
    static constexpr uint32_t _maxTextureSlots = 8;

    // Font
    std::vector<QuadVertex> _vertices;
    std::shared_ptr<Font> _currentFont = nullptr;

    // Defaults
    std::shared_ptr<Shader> _textShader = nullptr;

  private:
    void DrawText( //
      const std::string &text, glm::vec3 position, glm::vec4 color, float fontSize, const std::shared_ptr<Font> font,
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
