
#include "internal/rendering/VertexBuffer.hpp"
#include "glad/glad.h"
#include "internal/rendering/VertexBufferLayout.hpp"

namespace Base
{

  std::shared_ptr<VertexBuffer> VertexBuffer::Create(void *data, size_t size, BufferLayout layout)
  {
    return std::shared_ptr<VertexBuffer>(new VertexBuffer(data, size, layout));
  }

  std::shared_ptr<VertexBuffer> VertexBuffer::Create(size_t size, BufferLayout layout)
  {
    return std::shared_ptr<VertexBuffer>(new VertexBuffer(size, layout));
  }

  void VertexBuffer::Delete(std::shared_ptr<VertexBuffer> buffer)
  {
    glDeleteBuffers(1, buffer->_id.data());
  }

  VertexBuffer::VertexBuffer(                    //
    void *data, size_t size, BufferLayout layout //
    )
    : _layout(layout)
  {
    glGenBuffers(1, _id.data());
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  VertexBuffer::VertexBuffer(        //
    size_t size, BufferLayout layout //
    )
    : _layout(layout)
  {
    glGenBuffers(1, _id.data());
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void VertexBuffer::Bind()
  {
    glBindBuffer(GL_ARRAY_BUFFER, _id);
  }

  void VertexBuffer::Unbind()
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void VertexBuffer::Update(size_t size, void *data)
  {
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    Unbind();
  }

  const BufferLayout &VertexBuffer::GetLayout() const
  {
    return _layout;
  }

} // namespace Base
