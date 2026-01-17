#include "internal/rendering/batchers/TextBatcher.hpp"
#include "base/assets/Texture.hpp"
#include "base/rendering/GeometryType.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"
#include "internal/rendering/Vertex.hpp"
#include "internal/utils/MSDFData.hpp"
#include <cstdint>
#include <memory>
#include <unordered_set>

namespace Base
{
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

  void TextBatcher::Init()
  {
    _defaultShader = Shader::Create("", "", GeometryType::Text);
    _currentShader = _defaultShader;

    _geometryType = GeometryType::Text;

    _vertices.reserve(_maxVertices);
    _indices.reserve(_maxIndices);

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
  }

  void TextBatcher::DeInit()
  {
    Shader::Delete(_defaultShader);
    VertexArray::Delete(_vao);
    VertexBuffer::Delete(_vbo);
    IndexBuffer::Delete(_ibo);
  }

  void TextBatcher::Begin()
  {
    _vertices.clear();
    _currentIndex = 0;
  }

  void TextBatcher::DrawText( //
    const std::string &text, glm::vec3 position, glm::vec4 color, float fontSize, const std::shared_ptr<Font> font,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    if (_currentFont != font || _vertices.size() >= _maxVertices || _currentIndex >= _maxIndices)
    {
      Flush();
      Begin();
      _currentFont = font;
    }

    const auto &fontGeometry = font->GetMSDFData()->FontGeometry;
    const auto &metrics = fontGeometry.getMetrics();
    std::shared_ptr<Texture> fontAtlas = font->GetAtlas();
    float fontScale = fontSize / (metrics.ascenderY - metrics.descenderY);

    // Decode first character to get left offset
    const char *it = text.data();
    const char *end = it + text.size();
    char32_t firstChar;
    if (DecodeUTF8(it, end, firstChar))
    {
      auto firstGlyph = fontGeometry.getGlyph(firstChar);
      if (firstGlyph)
      {
        double firstLeft, firstBottom, firstRight, firstTop;
        firstGlyph->getQuadPlaneBounds(firstLeft, firstBottom, firstRight, firstTop);
        position.x = position.x - static_cast<float>(firstLeft * fontScale);
      }
    }

    float topLeftOffset = metrics.ascenderY;

    float penX = 0.0f;
    float penY = 0.0f; // baseline stays at position.y

    uint32_t attachmentMask = 0;
    for (auto attachment : attachments)
    {
      attachmentMask |= 1u << static_cast<uint8_t>(attachment);
    }

    // Reset iterator for main loop
    it = text.data();
    char32_t prevCharacter = 0;

    while (it < end)
    {
      char32_t character;
      if (!DecodeUTF8(it, end, character))
        break;

      // Handle newline
      if (character == U'\n')
      {
        penX = 0.0f;
        penY += metrics.lineHeight; // move down by line height
        prevCharacter = 0;
        continue;
      }
      else if (character == U'\t')
      {
        double spaceAdvance;
        fontGeometry.getAdvance(spaceAdvance, U' ', U' ');
        penX += spaceAdvance * 4.0f; // Tab = 4 spaces
        prevCharacter = 0;
        continue;
      }

      auto glyphGeometry = fontGeometry.getGlyph(character);
      if (!glyphGeometry)
      {
        glyphGeometry = fontGeometry.getGlyph(U'?');
        if (!glyphGeometry)
        {
          prevCharacter = character;
          continue;
        }
      }

      // Plane-space quad bounds (relative to baseline)
      double planeLeft, planeBottom, planeRight, planeTop;
      glyphGeometry->getQuadPlaneBounds(planeLeft, planeBottom, planeRight, planeTop);

      glm::vec2 planeMin = {planeLeft + penX, planeBottom - penY - topLeftOffset};
      glm::vec2 planeMax = {planeRight + penX, planeTop - penY - topLeftOffset};

      planeMin *= fontScale;
      planeMax *= fontScale;

      // Apply baseline position
      glm::vec3 bl = position + glm::vec3{planeMin.x, -planeMin.y, position.z}; // bottom-left
      glm::vec3 br = position + glm::vec3{planeMax.x, -planeMin.y, position.z}; // bottom-right
      glm::vec3 tl = position + glm::vec3{planeMin.x, -planeMax.y, position.z}; // top-left
      glm::vec3 tr = position + glm::vec3{planeMax.x, -planeMax.y, position.z}; // top-right

      // Atlas UVs
      double atlasLeft, atlasBottom, atlasRight, atlasTop;
      glyphGeometry->getQuadAtlasBounds(atlasLeft, atlasBottom, atlasRight, atlasTop);

      glm::vec2 atlasMin = {atlasLeft / fontAtlas->GetWidth(), atlasBottom / fontAtlas->GetHeight()};
      glm::vec2 atlasMax = {atlasRight / fontAtlas->GetWidth(), atlasTop / fontAtlas->GetHeight()};

      glm::vec3 uvBL = {atlasMin.x, atlasMin.y, 0.f};
      glm::vec3 uvBR = {atlasMax.x, atlasMin.y, 0.f};
      glm::vec3 uvTL = {atlasMin.x, atlasMax.y, 0.f};
      glm::vec3 uvTR = {atlasMax.x, atlasMax.y, 0.f};

      // Push quad
      _vertices.push_back({bl, uvBL, color, attachmentMask});
      _vertices.push_back({br, uvBR, color, attachmentMask});
      _vertices.push_back({tr, uvTR, color, attachmentMask});
      _vertices.push_back({tl, uvTL, color, attachmentMask});
      _currentIndex += 6;

      // Advance pen (with kerning)
      double advance;

      // Peek at next character for kerning
      const char *peekIt = it;
      char32_t nextCharacter = 0;
      if (peekIt < end)
      {
        DecodeUTF8(peekIt, end, nextCharacter);
      }

      fontGeometry.getAdvance(advance, character, nextCharacter);
      penX += advance;

      prevCharacter = character;
    }
  }

  void TextBatcher::Flush()
  {
    if (_vertices.empty())
    {
      return;
    }

    _vao->Bind();
    _vbo->Update(_vertices.size() * sizeof(QuadVertex), _vertices.data());
    _currentShader->SetUniform("uProjView", _currentCamera->GetProjView());
    _currentShader->Use();
    _currentFont->GetAtlas()->Bind(0);
    assert(_currentIndex <= _indices.size());
    assert((_currentIndex / 6) * 4 <= _vertices.size());

    glDrawElements(GL_TRIANGLES, _currentIndex, GL_UNSIGNED_SHORT, nullptr);
    _currentShader->Unuse();
    _vao->Unbind();
  }

  void TextBatcher::Submit(const RenderCommand &command)
  {
    if (std::holds_alternative<TextCommand>(command))
    {
      auto &com = std::get<TextCommand>(command);
      DrawText( //
        com.Text, com.Position, com.Color, com.FontSize, com.FontToUse,
        com.Attachments //
      );
    }
  }

} // namespace Base
