#pragma once

#include "base/rendering/RenderID.hpp"
#include "internal/rendering/IndexBuffer.hpp"
#include "internal/rendering/VertexBuffer.hpp"
#include <memory>
#include <vector>

namespace Base
{

  class VertexArray
  {
  private:
    RenderID _id = 0;
    std::vector<std::shared_ptr<VertexBuffer>> _buffers;
    std::shared_ptr<IndexBuffer> _indexBuffer;
    int _currentAttibuteIndex = 0;

    VertexArray();

  public:
    void Bind();
    void Unbind();

    void AddBuffer(const std::shared_ptr<VertexBuffer> &buffer);
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer);

    static std::shared_ptr<VertexArray> Create();
    static void Delete(std::shared_ptr<VertexArray> array);
  };

} // namespace Base
