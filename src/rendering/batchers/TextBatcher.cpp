#include "internal/rendering/batchers/TextBatcher.hpp"
#include "base/assets/Texture.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"
#include "internal/rendering/Vertex.hpp"
#include "internal/utils/MSDFData.hpp"
#include <cstdint>
#include <memory>
#include <unordered_set>

namespace Base
{

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
    const std::wstring &text, glm::vec3 position, glm::vec4 color, float fontSize, const std::shared_ptr<Font> font,
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

    auto firstGlyph = fontGeometry.getGlyph(text[0]);
    double firstLeft, firstBottom, firstRight, firstTop;
    firstGlyph->getQuadPlaneBounds(firstLeft, firstBottom, firstRight, firstTop);

    position.x = position.x - glm::vec2(firstLeft, 0).x;
    position.y = position.x - glm::vec2(firstLeft, 0).x;

    float topLeftOffset = metrics.ascenderY;

    float penX = 0.0f;
    float penY = 0.0f; // baseline stays at position.y

    uint32_t attachmentMask = 0;
    for (auto attachment : attachments)
    {
      attachmentMask |= 1u << static_cast<uint8_t>(attachment);
    }

    for (int i = 0; i < text.length(); i++)
    {
      char32_t character = text[i];

      // Handle newline
      if (character == L'\n')
      {
        penX = 0.0f;
        penY += metrics.lineHeight; // move down by line height
        continue;
      }
      else if (character == L'\t')
      {
        double spaceAdvance;
        fontGeometry.getAdvance(spaceAdvance, L' ', L' ');
        penX += spaceAdvance;

        fontGeometry.getAdvance(spaceAdvance, L' ', i < text.length() ? text[i + 1] : 0);
        penX += spaceAdvance;
      }

      auto glyphGeometry = fontGeometry.getGlyph(character);
      if (!glyphGeometry)
      {
        glyphGeometry = fontGeometry.getGlyph(L'?');
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

      // Advance pen
      double advance;
      wchar_t nextCharacter = i < text.length() ? text[i + 1] : 0;
      fontGeometry.getAdvance(advance, character, nextCharacter);
      penX += advance;
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
