#include "internal/rendering/VertexArray.hpp"
#include "glad/glad.h"
#include <memory>

namespace Base
{
  std::shared_ptr<VertexArray> VertexArray::Create()
  {
    return std::shared_ptr<VertexArray>(new VertexArray);
  }

  void VertexArray::Delete(std::shared_ptr<VertexArray> array)
  {
    glDeleteVertexArrays(1, array->_id.data());
  }

  VertexArray::VertexArray()
  {
    glGenVertexArrays(1, _id.data());
  }

  void VertexArray::AddBuffer(const std::shared_ptr<VertexBuffer> &buffer)
  {
    Bind();
    buffer->Bind();
    for (const BufferElement &element : buffer->GetLayout())
    {
      glEnableVertexAttribArray(_currentAttibuteIndex);

      if ( //
        element.Type == ShaderDataType::Int || element.Type == ShaderDataType::Int2 ||
        element.Type == ShaderDataType::Int3 || element.Type == ShaderDataType::Int4 ||
        element.Type == ShaderDataType::Uint || element.Type == ShaderDataType::Uint2 ||
        element.Type == ShaderDataType::Uint3 || element.Type == ShaderDataType::Uint4 //
      )
      {
        glVertexAttribIPointer( //
          _currentAttibuteIndex, element.GetComponentCount(), ShaderTypeToGLType(element.Type),
          buffer->GetLayout().GetStride(),
          (void *)element.Offset //
        );
      }
      else
      {
        glVertexAttribPointer( //
          _currentAttibuteIndex, element.GetComponentCount(), ShaderTypeToGLType(element.Type),
          element.Normalized ? GL_TRUE : GL_FALSE, buffer->GetLayout().GetStride(),
          (void *)element.Offset //
        );
      }
      _currentAttibuteIndex++;
    }
    _buffers.push_back(buffer);
    buffer->Unbind();
    Unbind();
  }

  void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
  {
    Bind();
    _indexBuffer = indexBuffer;
    _indexBuffer->Bind();
    Unbind();
  }

  void VertexArray::Bind()
  {
    glBindVertexArray(_id);
  }

  void VertexArray::Unbind()
  {
    glBindVertexArray(0);
  }

} // namespace Base
