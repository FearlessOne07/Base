#include "internal/rendering/IndexBuffer.hpp"
#include "glad/glad.h"

namespace Base
{
std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint16_t *data, size_t count)
{
  return std::shared_ptr<IndexBuffer>(new IndexBuffer(data, count));
}

void IndexBuffer::Delete(std::shared_ptr<IndexBuffer> buffer)
{
  glDeleteBuffers(1, buffer->_id.data());
}

IndexBuffer::IndexBuffer(uint16_t *data, int count) : _count(count)
{
  glGenBuffers(1, _id.data());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
  glBufferData( //
    GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * _count, data,
    GL_STATIC_DRAW //
  );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::Unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace Base
