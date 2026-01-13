#include "internal/rendering/batchers/QuadBatcher.hpp"
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "base/rendering/GeometryType.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>
#include <unordered_set>

namespace Base
{

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
    const std::shared_ptr<Texture> texture = _defaultQuadTexture;
    if (_currentTextureSlot >= _maxTextureSlots || _vertices.size() >= _maxVertices || _currentIndex >= _maxIndices)
    {
      Flush();
      Begin();
      _currentTextureSlot = 1;
    }

    uint32_t attachmentMask = 0;
    for (auto attachment : attachments)
    {
      attachmentMask |= 1u << static_cast<uint8_t>(attachment);
    }

    glm::vec2 size = quad.GetSize();

    glm::vec3 corners[4] = {
      {position.x, position.y, position.z},
      {position.x + size.x, position.y, position.z},
      {position.x + size.x, position.y + size.y, position.z},
      {position.x, position.y + size.y, position.z},
    };

    glm::vec2 pivot;
    switch (quad.GetOrigin())
    {
    case Origin::Center:
      pivot = glm::vec2(position.x, position.y) + (size * 0.5f);
      break;
    case Origin::TopLeft:
      pivot = position;
      break;
    }
    glm::mat4 rotationMat = glm::translate(glm::mat4(1.0f), glm::vec3(pivot, 0.0f)) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
                            glm::translate(glm::mat4(1.0f), glm::vec3(-pivot, 0.0f));
    for (int i = 0; i < 4; i++)
    {
      corners[i] = rotationMat * glm::vec4(corners[i], 1.f);
    }

    _vertices.push_back({corners[0], {0.0f, 0.0f, 0.f}, color, attachmentMask});
    _vertices.push_back({corners[1], {1.0f, 0.0f, 0.f}, color, attachmentMask});
    _vertices.push_back({corners[2], {1.0f, 1.0f, 0.f}, color, attachmentMask});
    _vertices.push_back({corners[3], {0.0f, 1.0f, 0.f}, color, attachmentMask});
    _currentIndex += 6;
  }

  void QuadBatcher::DrawQuad( //
    const Sprite &sprite, glm::vec3 position, glm::vec2 size, float rotation,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    if (!sprite.HasMaterial())
    {
      auto texture = sprite.GetTexture();
      if (_currentTextureSlot >= _maxTextureSlots || _vertices.size() >= _maxVertices || _currentIndex >= _maxIndices)
      {
        Flush();
        Begin();
      }

      float slot = 0;
      uint32_t attachmentMask = 0;
      for (auto attachment : attachments)
      {
        attachmentMask |= 1u << static_cast<uint8_t>(attachment);
      }

      if (auto it = std::ranges::find(_boundTextures, texture); it == _boundTextures.end())
      {
        slot = _currentTextureSlot++;
        _boundTextures[slot] = texture;
      }
      else
      {
        slot = std::distance(_boundTextures.begin(), it);
      }

      glm::vec2 texCoordMin = sprite.GetSourcePos() / glm::vec2{texture->GetWidth(), texture->GetHeight()};
      glm::vec2 texSize = sprite.GetSourceSize() / glm::vec2{texture->GetWidth(), texture->GetHeight()};

      texCoordMin.y = 1.0f - texCoordMin.y - texSize.y;

      glm::vec2 texCoordMax = texCoordMin + texSize;
      glm::vec3 corners[4] = {
        {position.x, position.y, position.z},
        {position.x + size.x, position.y, position.z},
        {position.x + size.x, position.y + size.y, position.z},
        {position.x, position.y + size.y, position.z},
      };
      glm::vec2 pivot;
      switch (sprite.GetOrigin())
      {
      case Origin::Center:
        pivot = glm::vec2(position.x, position.y) + (size * 0.5f);
        break;
      case Origin::TopLeft:
        pivot = position;
        break;
      }
      glm::mat4 rotationMat = glm::translate(glm::mat4(1.0f), glm::vec3(pivot, 0.0f)) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
                              glm::translate(glm::mat4(1.0f), glm::vec3(-pivot, 0.0f));

      for (int i = 0; i < 4; i++)
      {
        corners[i] = rotationMat * glm::vec4(corners[i], 1.f);
      }

      _vertices.push_back({corners[0], {texCoordMin.x, texCoordMax.y, slot}, {1.f, 1.f, 1.f, 1.f}, attachmentMask});
      _vertices.push_back({corners[1], {texCoordMax.x, texCoordMax.y, slot}, {1.f, 1.f, 1.f, 1.f}, attachmentMask});
      _vertices.push_back({corners[2], {texCoordMax.x, texCoordMin.y, slot}, {1.f, 1.f, 1.f, 1.f}, attachmentMask});
      _vertices.push_back({corners[3], {texCoordMin.x, texCoordMin.y, slot}, {1.f, 1.f, 1.f, 1.f}, attachmentMask});
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
    const auto &texture = sprite.GetTexture();

    mat.SetUniform("uProjView", _currentCamera->GetProjView());
    mat.Bind();

    uint32_t attachmentMask = 0;
    for (auto attachment : attachments)
    {
      attachmentMask |= 1u << static_cast<uint8_t>(attachment);
    }

    glm::vec2 texMin = sprite.GetSourcePos() / glm::vec2(texture->GetWidth(), texture->GetHeight());
    glm::vec2 texSize = sprite.GetSourceSize() / glm::vec2(texture->GetWidth(), texture->GetHeight());

    texMin.y = 1.f - texMin.y - texSize.y;
    glm::vec2 texMax = texMin + texSize;

    glm::vec3 corners[4] = {
      {position.x, position.y, position.z},
      {position.x + size.x, position.y, position.z},
      {position.x + size.x, position.y + size.y, position.z},
      {position.x, position.y + size.y, position.z},
    };
    glm::vec2 pivot;
    switch (sprite.GetOrigin())
    {
    case Origin::Center:
      pivot = glm::vec2(position) + (size * 0.5f);
    case Origin::TopLeft:
      glm::vec2 pivot = position;
      break;
    }
    glm::mat4 rotationMat = glm::translate(glm::mat4(1.0f), glm::vec3(pivot, 0.0f)) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
                            glm::translate(glm::mat4(1.0f), glm::vec3(-pivot, 0.0f));

    for (int i = 0; i < 4; i++)
    {
      corners[i] = rotationMat * glm::vec4(corners[i], 1.f);
    }

    QuadVertex quadVerts[4] = {
      {corners[0], {texMin.x, texMax.y, 0.f}, {1, 1, 1, 1}, attachmentMask},
      {corners[1], {texMax.x, texMax.y, 0.f}, {1, 1, 1, 1}, attachmentMask},
      {corners[2], {texMax.x, texMin.y, 0.f}, {1, 1, 1, 1}, attachmentMask},
      {corners[3], {texMin.x, texMin.y, 0.f}, {1, 1, 1, 1}, attachmentMask},
    };

    _materialVao->Bind();
    _materialVbo->Update(sizeof(quadVerts), quadVerts);

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
    _currentTexture->Bind(0);
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
        com.SpriteToDraw, com.Position, com.Size, com.Rotation,
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
