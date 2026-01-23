#pragma once

#include "base/rendering/RenderID.hpp"
#include <cstdint>
#include <memory>

namespace Base
{

  class IndexBuffer
  {
  private:
    RenderID _id = 0;
    unsigned int _count;

  private:
    IndexBuffer(uint16_t *data, int count);

  public:
    static std::shared_ptr<IndexBuffer> Create(uint16_t *data, size_t count);
    static void Delete(std::shared_ptr<IndexBuffer> buffer);
    IndexBuffer() = default;
    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const
    {
      return _count;
    };
  };

} // namespace Base
