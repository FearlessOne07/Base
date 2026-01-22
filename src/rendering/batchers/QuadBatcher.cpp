#include "internal/rendering/batchers/QuadBatcher.hpp"
#include "base/util/Type.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>
#include <unordered_set>

namespace Base
{
  static void BuildQuadCorners(glm::vec3 out[4], glm::vec3 position, glm::vec2 size, Origin origin, float rotationDeg)
  {
    const glm::vec2 half = size * 0.5f;

    // Local-space quad centered at origin
    glm::vec3 local[4] = {
      {-half.x, -half.y, 0.f},
      {half.x, -half.y, 0.f},
      {half.x, half.y, 0.f},
      {-half.x, half.y, 0.f},
    };

    // If top-left, shift quad so pivot is top-left
    if (origin == Origin::TopLeft)
    {
      for (auto &v : local)
        v += glm::vec3(half, 0.f);
    }

    // World transform (pivot is always position)
    glm::mat4 transform =
      glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), glm::radians(rotationDeg), {0, 0, 1});

    for (int i = 0; i < 4; i++)
      out[i] = transform * glm::vec4(local[i], 1.f);
  }
  void QuadBatcher::Init()
  {
    QuadVertex::Init();

    uint32_t data = 0xFFFFFFFF;
    _defaultQuadTexture = Texture::Create(&data, TextureSpec{.Width = 1, .Height = 1});
    _defaultShader = Shader::Create("", "", GeometryType::Quad);
    _currentShader = _defaultShader;

    _geometryType = GeometryType::Quad;

    _vertices.reserve(_maxVertices);
    _indices.reserve(_maxIndices);

    _boundTextures[0] = _defaultQuadTexture;

    _vbo = VertexBuffer::Create(_maxVertices * sizeof(QuadVertex), QuadVertex::GetLayout());

    for (int i = 0; i < _maxQuads; i++)
    {
      uint16_t offset = i * 4;
      _indices.insert(  //
        _indices.end(), //
        {
          static_cast<uint16_t>(offset + 0),
          static_cast<uint16_t>(offset + 1),
          static_cast<uint16_t>(offset + 2),
          static_cast<uint16_t>(offset + 0),
          static_cast<uint16_t>(offset + 2),
          static_cast<uint16_t>(offset + 3),
        } //
      );
    }
    _ibo = IndexBuffer::Create(_indices.data(), _indices.size());

    _vao = VertexArray::Create();
    _vao->AddBuffer(_vbo);
    _vao->SetIndexBuffer(_ibo);

    uint16_t quadIndices[6] = {0, 1, 2, 0, 2, 3};

    // Material Buffers
    _materialVbo = VertexBuffer::Create(sizeof(QuadVertex) * 4, QuadVertex::GetLayout());
    _materialIbo = IndexBuffer::Create(quadIndices, 6);
    _materialVao = VertexArray::Create();

    _materialVao->AddBuffer(_materialVbo);
    _materialVao->SetIndexBuffer(_materialIbo);

    _currentTexture = _defaultQuadTexture;
  }

  void QuadBatcher::DeInit()
  {
    Texture::Destroy(_defaultQuadTexture);
    Shader::Delete(_defaultShader);
    VertexArray::Delete(_vao);
    VertexBuffer::Delete(_vbo);
    IndexBuffer::Delete(_ibo);

    VertexBuffer::Delete(_materialVbo);
    IndexBuffer::Delete(_materialIbo);
    VertexArray::Delete(_materialVao);
  }

  void QuadBatcher::SetMaxTextureSlots(uint8_t slots)
  {
    _maxTextureSlots = slots;
  }

  void QuadBatcher::Begin()
  {
    _vertices.clear();
    _currentIndex = 0;
    _currentTextureSlot = 1;

    _boundTextures.fill(nullptr);
    _boundTextures[0] = _defaultQuadTexture;
  }

  void QuadBatcher::DrawQuad( //
    const Rectangle &quad, glm::vec3 position, glm::vec4 color, float rotation,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    if (_currentTextureSlot >= _maxTextureSlots || _vertices.size() + 4 > _maxVertices ||
        _currentIndex + 6 > _maxIndices)
    {
      Flush();
      Begin();
    }

    uint32_t attachmentMask = 0;
    for (auto a : attachments)
      attachmentMask |= 1u << static_cast<uint8_t>(a);

    glm::vec3 corners[4];
    BuildQuadCorners(corners, position, quad.GetSize(), quad.GetOrigin(), rotation);

    _vertices.push_back({corners[0], {0, 0, 0}, color, attachmentMask});
    _vertices.push_back({corners[1], {1, 0, 0}, color, attachmentMask});
    _vertices.push_back({corners[2], {1, 1, 0}, color, attachmentMask});
    _vertices.push_back({corners[3], {0, 1, 0}, color, attachmentMask});

    _currentIndex += 6;
  }

  void QuadBatcher::DrawQuad( //
    const Sprite &sprite, glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    if (!sprite.HasMaterial())
    {
      auto texture = sprite.GetTexture();

      if (_currentTextureSlot >= _maxTextureSlots || _vertices.size() + 4 > _maxVertices ||
          _currentIndex + 6 > _maxIndices)
      {
        Flush();
        Begin();
      }

      uint32_t attachmentMask = 0;
      for (auto a : attachments)
        attachmentMask |= 1u << static_cast<uint8_t>(a);

      float slot;
      if (auto it = std::ranges::find(_boundTextures, texture); it == _boundTextures.end())
      {
        slot = (float)_currentTextureSlot;
        _boundTextures[_currentTextureSlot++] = texture;
      }
      else
      {
        slot = (float)std::distance(_boundTextures.begin(), it);
      }

      glm::vec2 texMin = sprite.GetSourcePos() / glm::vec2(texture->GetWidth(), texture->GetHeight());
      glm::vec2 texSize = sprite.GetSourceSize() / glm::vec2(texture->GetWidth(), texture->GetHeight());

      texMin.y = 1.f - texMin.y - texSize.y;
      glm::vec2 texMax = texMin + texSize;

      glm::vec3 corners[4];
      BuildQuadCorners(corners, position, size, sprite.GetOrigin(), rotation);

      _vertices.push_back({corners[0], {texMin.x, texMax.y, slot}, color, attachmentMask});
      _vertices.push_back({corners[1], {texMax.x, texMax.y, slot}, color, attachmentMask});
      _vertices.push_back({corners[2], {texMax.x, texMin.y, slot}, color, attachmentMask});
      _vertices.push_back({corners[3], {texMin.x, texMin.y, slot}, color, attachmentMask});

      _currentIndex += 6;
    }
    else
    {
      Flush();
      DrawMaterialedQuad(position, size, sprite, rotation, attachments);
      Begin();
    }
  }

  void QuadBatcher::DrawMaterialedQuad( //
    glm::vec3 position, glm::vec2 size, const Sprite &sprite, float rotation,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    auto mat = sprite.GetMaterial();
    auto texture = sprite.GetTexture();

    mat.SetUniform("uProjView", _currentCamera->GetProjView());
    mat.Bind();

    uint32_t attachmentMask = 0;
    for (auto a : attachments)
      attachmentMask |= 1u << static_cast<uint8_t>(a);

    glm::vec2 texMin = sprite.GetSourcePos() / glm::vec2(texture->GetWidth(), texture->GetHeight());
    glm::vec2 texSize = sprite.GetSourceSize() / glm::vec2(texture->GetWidth(), texture->GetHeight());

    texMin.y = 1.f - texMin.y - texSize.y;
    glm::vec2 texMax = texMin + texSize;

    glm::vec3 corners[4];
    BuildQuadCorners(corners, position, size, sprite.GetOrigin(), rotation);

    QuadVertex verts[4] = {
      {corners[0], {texMin.x, texMax.y, 0}, {1, 1, 1, 1}, attachmentMask},
      {corners[1], {texMax.x, texMax.y, 0}, {1, 1, 1, 1}, attachmentMask},
      {corners[2], {texMax.x, texMin.y, 0}, {1, 1, 1, 1}, attachmentMask},
      {corners[3], {texMin.x, texMin.y, 0}, {1, 1, 1, 1}, attachmentMask},
    };

    _materialVao->Bind();
    _materialVbo->Update(sizeof(verts), verts);
    texture->Bind(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    mat.Unbind();
    _materialVao->Unbind();
  }

  void QuadBatcher::Flush()
  {
    if (_vertices.empty())
      return;

    _vao->Bind();
    _vbo->Update(_vertices.size() * sizeof(QuadVertex), _vertices.data());
    _currentShader->SetUniform("uProjView", _currentCamera->GetProjView());
    _currentShader->Use();

    _defaultQuadTexture->Bind(0);
    for (int i = 1; i < _currentTextureSlot; i++)
    {
      _boundTextures[i]->Bind(i);
    }
    // _currentTexture->Bind(0);
    glDrawElements(GL_TRIANGLES, _currentIndex, GL_UNSIGNED_SHORT, nullptr);
    _currentShader->Unuse();
    _vao->Unbind();
  }

  void QuadBatcher::Submit(const RenderCommand &command)
  {
    if (std::holds_alternative<SpriteCommand>(command))
    {
      auto &com = std::get<SpriteCommand>(command);
      DrawQuad( //
        com.SpriteToDraw, com.Position, com.Size, com.Rotation, com.Color,
        com.Attachments //
      );
    }
    else if (std::holds_alternative<QuadCommand>(command))
    {
      auto &com = std::get<QuadCommand>(command);
      DrawQuad( //
        com.QuadToDraw, com.Position, com.Color, com.Rotation,
        com.Attachments //
      );
    }
  }

} // namespace Base
