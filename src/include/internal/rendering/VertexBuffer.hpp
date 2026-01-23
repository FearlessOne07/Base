#pragma once
#include "base/rendering/RenderID.hpp"
#include "internal/rendering/VertexBufferLayout.hpp"
#include <memory>

namespace Base
{

  class VertexBuffer
  {
  private:
    RenderID _id = 0;
    BufferLayout _layout;

  private:
    VertexBuffer(void *data, size_t size, BufferLayout layout);
    VertexBuffer(size_t size, BufferLayout layout);

  public:
    VertexBuffer() = default;
    void Bind();
    void Unbind();
    void Update(size_t size, void *data);

    const BufferLayout &GetLayout() const;

    static std::shared_ptr<VertexBuffer> Create(void *data, size_t size, BufferLayout layout);
    static std::shared_ptr<VertexBuffer> Create(size_t size, BufferLayout layout);
    static void Delete(std::shared_ptr<VertexBuffer> buffer);
  };

} // namespace Base
