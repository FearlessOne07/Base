#include "internal/rendering/batchers/TextureBatcher.hpp"
#include "base/util/Type.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "internal/utils/MSDFData.hpp"
#include <algorithm>
#include <unordered_set>

namespace Base
{

  void TextureBatcher::EmitQuad( //
    const glm::vec3 corners[4], const glm::vec2 texMin, const glm::vec2 texMax, uint32_t slot, TextureMode mode,
    const glm::vec4 &color, uint32_t attachmentMask //
  )
  {
    _vertices.push_back({corners[0], {texMin.x, texMax.y, slot}, static_cast<uint32_t>(mode), color, attachmentMask});
    _vertices.push_back({corners[1], {texMax.x, texMax.y, slot}, static_cast<uint32_t>(mode), color, attachmentMask});
    _vertices.push_back({corners[2], {texMax.x, texMin.y, slot}, static_cast<uint32_t>(mode), color, attachmentMask});
    _vertices.push_back({corners[3], {texMin.x, texMin.y, slot}, static_cast<uint32_t>(mode), color, attachmentMask});
    _currentIndex += 6;
  }

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

  uint32_t TextureBatcher::ResolveTextureSlot(const std::shared_ptr<Texture> &texture)
  {
    // Try to find existing
    auto it = std::ranges::find(_boundTextures, texture);
    if (it != _boundTextures.end())
      return static_cast<uint32_t>(std::distance(_boundTextures.begin(), it));

    // Need a new slot → flush if full
    if (_currentTextureSlot >= _maxTextureSlots)
    {
      Flush();
      Begin();
    }

    uint32_t slot = _currentTextureSlot;
    _boundTextures[_currentTextureSlot++] = texture;
    return slot;
  }

  void TextureBatcher::Init()
  {
    TextureVertex::Init();

    uint32_t data = 0xFFFFFFFF;
    _defaultQuadTexture = Texture::Create(&data, TextureSpec{.Width = 1, .Height = 1});
    _defaultShader = Shader::Create("", "", GeometryType::Texture);
    _currentShader = _defaultShader;

    _geometryType = GeometryType::Texture;

    _vertices.reserve(_maxVertices);
    _indices.reserve(_maxIndices);

    _boundTextures[0] = _defaultQuadTexture;

    _vbo = VertexBuffer::Create(_maxVertices * sizeof(TextureVertex), TextureVertex::GetLayout());

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
    _materialVbo = VertexBuffer::Create(sizeof(TextureVertex) * 4, TextureVertex::GetLayout());
    _materialIbo = IndexBuffer::Create(quadIndices, 6);
    _materialVao = VertexArray::Create();

    _materialVao->AddBuffer(_materialVbo);
    _materialVao->SetIndexBuffer(_materialIbo);

    _currentTexture = _defaultQuadTexture;
  }

  void TextureBatcher::DeInit()
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

  void TextureBatcher::SetMaxTextureSlots(uint8_t slots)
  {
    _maxTextureSlots = slots;
  }

  void TextureBatcher::Begin()
  {
    _vertices.clear();
    _currentIndex = 0;
    _currentTextureSlot = 1;

    _boundTextures.fill(nullptr);
    _boundTextures[0] = _defaultQuadTexture;
  }

  void TextureBatcher::DrawQuad( //
    const Rectangle &quad, glm::vec3 position, glm::vec4 color, float rotation,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    if (_vertices.size() + 4 > _maxVertices || _currentIndex + 6 > _maxIndices)
    {
      Flush();
      Begin();
    }

    uint32_t attachmentMask = 0;
    for (auto a : attachments)
      attachmentMask |= 1u << static_cast<uint8_t>(a);

    glm::vec3 corners[4];
    BuildQuadCorners(corners, position, quad.GetSize(), quad.GetOrigin(), rotation);
    EmitQuad(corners, {0, 0}, {1, 1}, 0, TextureMode::Sprite, color, attachmentMask);
  }

  void TextureBatcher::DrawQuad( //
    const Sprite &sprite, glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    if (!sprite.HasMaterial())
    {
      auto texture = sprite.GetTexture();

      if (_vertices.size() + 4 > _maxVertices || _currentIndex + 6 > _maxIndices)
      {
        Flush();
        Begin();
      }

      uint32_t attachmentMask = 0;
      for (auto a : attachments)
        attachmentMask |= 1u << static_cast<uint8_t>(a);

      float slot = ResolveTextureSlot(texture);

      glm::vec2 texMin = sprite.GetSourcePos() / glm::vec2(texture->GetWidth(), texture->GetHeight());
      glm::vec2 texSize = sprite.GetSourceSize() / glm::vec2(texture->GetWidth(), texture->GetHeight());

      texMin.y = 1.f - texMin.y - texSize.y;
      glm::vec2 texMax = texMin + texSize;

      glm::vec3 corners[4];
      BuildQuadCorners(corners, position, size, sprite.GetOrigin(), rotation);
      EmitQuad(corners, texMin, texMax, slot, TextureMode::Sprite, color, attachmentMask);
    }
    else
    {
      Flush();
      DrawMaterialedQuad(position, size, sprite, rotation, attachments);
      Begin();
    }
  }

  void TextureBatcher::DrawMaterialedQuad( //
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

    TextureVertex verts[4] = {
      {corners[0], {texMin.x, texMax.y, 0}, static_cast<uint32_t>(TextureMode::Sprite), {1, 1, 1, 1}, attachmentMask},
      {corners[1], {texMax.x, texMax.y, 0}, static_cast<uint32_t>(TextureMode::Sprite), {1, 1, 1, 1}, attachmentMask},
      {corners[2], {texMax.x, texMin.y, 0}, static_cast<uint32_t>(TextureMode::Sprite), {1, 1, 1, 1}, attachmentMask},
      {corners[3], {texMin.x, texMin.y, 0}, static_cast<uint32_t>(TextureMode::Sprite), {1, 1, 1, 1}, attachmentMask},
    };

    _materialVao->Bind();
    _materialVbo->Update(sizeof(verts), verts);
    texture->Bind(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    mat.Unbind();
    _materialVao->Unbind();
  }

  void TextureBatcher::Flush()
  {
    if (_vertices.empty())
      return;

    _vao->Bind();
    _vbo->Update(_vertices.size() * sizeof(TextureVertex), _vertices.data());
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

  void TextureBatcher::Submit(const RenderCommand &command)
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
    else if (std::holds_alternative<TextCommand>(command))
    {
      auto &com = std::get<TextCommand>(command);
      DrawText( //
        com.Text, com.Position, com.Color, com.FontSize, com.FontToUse,
        com.Attachments //
      );
    }
  }

  //// TEXT//////

  // UTF-8 decoder
  static bool DecodeUTF8(const char *&it, const char *end, char32_t &out)
  {
    if (it >= end)
      return false;

    unsigned char c = static_cast<unsigned char>(*it++);

    // 1-byte (ASCII)
    if (c < 0x80)
    {
      out = c;
      return true;
    }

    // 2-byte
    if ((c >> 5) == 0x6 && it < end)
    {
      out = ((c & 0x1F) << 6) | (static_cast<unsigned char>(*it++) & 0x3F);
      return true;
    }

    // 3-byte
    if ((c >> 4) == 0xE && it + 1 < end)
    {
      out = ((c & 0x0F) << 12) | ((static_cast<unsigned char>(*it++) & 0x3F) << 6) |
            (static_cast<unsigned char>(*it++) & 0x3F);
      return true;
    }

    // 4-byte
    if ((c >> 3) == 0x1E && it + 2 < end)
    {
      out = ((c & 0x07) << 18) | ((static_cast<unsigned char>(*it++) & 0x3F) << 12) |
            ((static_cast<unsigned char>(*it++) & 0x3F) << 6) | (static_cast<unsigned char>(*it++) & 0x3F);
      return true;
    }

    // Invalid UTF-8 → replacement character
    out = U'\uFFFD';
    return true;
  }

  void TextureBatcher::DrawText( //
    const std::string &text, glm::vec3 position, glm::vec4 color, float fontSize, const std::shared_ptr<Font> font,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    if (_vertices.size() + 4 > _maxVertices || _currentIndex + 6 > _maxIndices)
    {
      Flush();
      Begin();
    }

    if (text.empty() || !font)
      return;

    uint32_t slot = ResolveTextureSlot(font->GetAtlas());

    const auto &msdfData = *font->GetMSDFData();
    const auto &fontGeometry = msdfData.FontGeometry;
    const auto &metrics = fontGeometry.getMetrics();
    std::shared_ptr<Texture> fontAtlas = font->GetAtlas();
    float scaleFactor = fontSize / (float)(metrics.ascenderY - metrics.descenderY);

    uint32_t attachmentMask = 0;
    for (auto attachment : attachments)
    {
      attachmentMask |= 1u << static_cast<uint8_t>(attachment);
    }

    float initialYOffset = (float)metrics.ascenderY * scaleFactor;

    std::vector<char32_t> codepoints;
    const char *it = text.data();
    const char *end = it + text.size();
    char32_t c;
    while (DecodeUTF8(it, end, c))
      codepoints.push_back(c);

    auto firstGlyph = fontGeometry.getGlyph(codepoints[0]);
    double pl, pb, pr, pt;
    firstGlyph->getQuadPlaneBounds(pl, pb, pr, pt);

    float leftOverhang = (float)pl;

    float cursorX = position.x - leftOverhang;
    float cursorY = position.y;

    for (size_t i = 0; i < codepoints.size(); ++i)
    {
      char32_t character = codepoints[i];

      if (character == U'\n')
      {
        cursorX = position.x;
        cursorY += fontSize;
        continue;
      }
      else if (character == U'\t')
      {
        double spaceAdvance;
        fontGeometry.getAdvance(spaceAdvance, U' ', U' ');
        cursorX += (float)spaceAdvance * scaleFactor * 4.0f;
        continue;
      }

      auto glyph = fontGeometry.getGlyph(character);
      if (!glyph)
        glyph = fontGeometry.getGlyph(U'?');
      if (!glyph)
        continue;

      double pl, pb, pr, pt;
      glyph->getQuadPlaneBounds(pl, pb, pr, pt);

      float x0 = cursorX + (float)pl * scaleFactor;
      float y0 = cursorY + (initialYOffset - (float)pt * scaleFactor);
      float x1 = cursorX + (float)pr * scaleFactor;
      float y1 = cursorY + (initialYOffset - (float)pb * scaleFactor);

      double al, ab, ar, at;
      glyph->getQuadAtlasBounds(al, ab, ar, at);
      float texW = (float)fontAtlas->GetWidth();
      float texH = (float)fontAtlas->GetHeight();

      glm::vec2 uvMin = {(float)al / texW, (float)at / texH};
      glm::vec2 uvMax = {(float)ar / texW, (float)ab / texH};
      glm::vec3 corners[4] = {
        {x0, y1, position.z}, // bottom-left  (was corners[1])
        {x1, y1, position.z}, // bottom-right (was corners[2])
        {x1, y0, position.z}, // top-right    (was corners[3])
        {x0, y0, position.z}, // top-left     (was corners[0])
      };

      EmitQuad(corners, uvMin, uvMax, slot, TextureMode::Text, color, attachmentMask);

      double advance;
      if (i + 1 < codepoints.size() && codepoints[i + 1] != U'\n')
      {
        char32_t nextChar = codepoints[i + 1];
        fontGeometry.getAdvance(advance, character, nextChar);
      }
      else
      {
        advance = glyph->getAdvance();
      }

      cursorX += (float)advance * scaleFactor;
    }
  }
} // namespace Base
