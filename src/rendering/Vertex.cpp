
#include "internal/rendering/Vertex.hpp"

namespace Base
{

  void TextureVertex::Init()
  {
    _layout = {{
      {ShaderDataType::Float3},
      {ShaderDataType::Float3},
      {ShaderDataType::Int},
      {ShaderDataType::Float4},
      {ShaderDataType::Int},
    }};
  }

  const BufferLayout &TextureVertex::GetLayout()
  {
    return _layout;
  }

  void CircleVertex::Init()
  {
    _layout = {{
      {ShaderDataType::Float3},
      {ShaderDataType::Float4},
      {ShaderDataType::Float2},
      {ShaderDataType::Float2},
      {ShaderDataType::Int},
    }};
  }

  const BufferLayout &CircleVertex::GetLayout()
  {
    return _layout;
  }

} // namespace Base
