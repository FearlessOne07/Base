#pragma once
#include "base/assets/Texture.hpp"
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "base/rendering/Quad.hpp"
#include "base/rendering/Sprite.hpp"
#include "glm/fwd.hpp"
#include "internal/rendering/Vertex.hpp"
#include "internal/rendering/batchers/Batcher.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <unordered_set>

namespace Base
{

  class QuadBatcher : public Batcher
  {
  private:
    static constexpr uint32_t _maxQuads = 1000;
    static constexpr uint32_t _maxVertices = _maxQuads * 4;
    static constexpr uint32_t _maxIndices = _maxQuads * 6;

    uint32_t _maxTextureSlots = 8;

    // Textures
    std::vector<QuadVertex> _vertices;
    std::shared_ptr<Texture> _currentTexture = nullptr;
    int _currentTextureSlot = 1;

    // Defaults
    std::shared_ptr<Texture> _defaultQuadTexture = nullptr;
    std::array<std::shared_ptr<Texture>, 16> _boundTextures;

    std::shared_ptr<VertexBuffer> _materialVbo;
    std::shared_ptr<IndexBuffer> _materialIbo;
    std::shared_ptr<VertexArray> _materialVao;

  private:
    void DrawMaterialedQuad( //
      glm::vec3 position, glm::vec2 size, const Sprite &sprite, float rotation,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments //
    );

    void DrawQuad( //
      const Rectangle &quad, glm::vec3 position, glm::vec4 color, float rotationDeg,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments //
    );
    void DrawQuad( //
      const Sprite &sprite, glm::vec3 position, glm::vec2 size, float rotationDeg,
      const std::unordered_set<FramebufferAttachmentIndex> &attachments //
    );

  public:
    void Init() override;
    void DeInit() override;
    void Begin() override;
    void Flush() override;
    void Submit(const RenderCommand &command) override;

    // Shaders
    void SetMaxTextureSlots(uint8_t slots);
  };

} // namespace Base
