
#include "internal/rendering/batchers/CircleBatcher.hpp"
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "base/rendering/GeometryType.hpp"
#include <algorithm>
#include <unordered_set>

namespace Base
{

  void CircleBatcher::Init()
  {
    CircleVertex::Init();

    _defaultShader = Shader::Create("", "", GeometryType::Circle);
    _currentShader = _defaultShader;

    _vertices.reserve(_maxVertices);
    _indices.reserve(_maxIndices);

    _geometryType = GeometryType::Circle;

    _vbo = VertexBuffer::Create(_maxVertices * sizeof(CircleVertex), CircleVertex::GetLayout());

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

  void CircleBatcher::DeInit()
  {
    Shader::Delete(_defaultShader);
    VertexArray::Delete(_vao);
    VertexBuffer::Delete(_vbo);
    IndexBuffer::Delete(_ibo);
  }

  void CircleBatcher::Begin()
  {
    _vertices.clear();
    _currentIndex = 0;
  }

  void CircleBatcher::Flush()
  {
    if (_vertices.empty())
      return;

    _vao->Bind();
    _vbo->Update(_vertices.size() * sizeof(CircleVertex), _vertices.data());
    _currentShader->SetUniform("uProjView", _currentCamera->GetProjView());
    _currentShader->Use();
    glDrawElements(GL_TRIANGLES, _currentIndex, GL_UNSIGNED_SHORT, nullptr);
    _currentShader->Unuse();
    _vao->Unbind();
  }

  void CircleBatcher::DrawCircle( //
    glm::vec3 position, float radius, glm::vec4 color, float thickness,
    const std::unordered_set<FramebufferAttachmentIndex> &attachments //
  )
  {
    if (_vertices.size() >= _maxVertices || _currentIndex >= _maxIndices)
    {
      Flush();
      Begin();
    }

    uint32_t attachmentMask = 0;
    for (auto attachment : attachments)
    {
      attachmentMask |= 1u << static_cast<uint8_t>(attachment);
    }
    float thicknessUV = std::clamp<float>(thickness / radius, 0.f, 1.f);
    float fade = 2.1f / radius;

    _vertices.push_back({
      position - glm::vec3(glm::vec2(radius), 0.f),
      color,
      {0.0f, 0.0f},
      {thicknessUV, fade},
      attachmentMask,
    });
    _vertices.push_back({
      {position.x + radius, position.y - radius, position.z},
      color,
      {1.0f, 0.0f},
      {thicknessUV, fade},
      attachmentMask,
    });
    _vertices.push_back({
      {position + glm::vec3(glm::vec2(radius), 0)},
      color,
      {1.0f, 1.0f},
      {thicknessUV, fade},
      attachmentMask,
    });
    _vertices.push_back({
      {position.x - radius, position.y + radius, position.z},
      color,
      {0.0f, 1.0f},
      {thicknessUV, fade},
      attachmentMask,
    });
    _currentIndex += 6;
  }

  void CircleBatcher::Submit(const RenderCommand &command)
  {
    if (std::holds_alternative<CircleCommand>(command))
    {
      auto &com = std::get<CircleCommand>(command);
      DrawCircle( //
        com.Position, com.Radius, com.Color, com.Thickness,
        com.Attachments //
      );
    }
  }

} // namespace Base
