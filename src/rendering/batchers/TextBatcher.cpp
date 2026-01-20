#include "internal/rendering/batchers/TextBatcher.hpp"
#include "internal/utils/MSDFData.hpp"

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
    if (text.empty() || !font)
      return;

    if (_currentFont != font || _vertices.size() + (text.size() * 4) >= _maxVertices)
    {
      Flush();
      Begin();
      _currentFont = font;
    }

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

      glm::vec2 uvMin = {(float)al / texW, (float)ab / texH};
      glm::vec2 uvMax = {(float)ar / texW, (float)at / texH};

      _vertices.push_back({{x0, y0, position.z}, {uvMin.x, uvMax.y, 0.f}, color, attachmentMask}); // TL
      _vertices.push_back({{x0, y1, position.z}, {uvMin.x, uvMin.y, 0.f}, color, attachmentMask}); // BL
      _vertices.push_back({{x1, y1, position.z}, {uvMax.x, uvMin.y, 0.f}, color, attachmentMask}); // BR
      _vertices.push_back({{x1, y0, position.z}, {uvMax.x, uvMax.y, 0.f}, color, attachmentMask}); // TR

      _currentIndex += 6;

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
