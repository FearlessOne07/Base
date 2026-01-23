#pragma once
#include <cstdint>

namespace Base
{

  class RenderID
  {
  private:
    uint32_t _id = 0;

  public:
    RenderID();
    RenderID(uint32_t);
    operator bool();
    operator uint32_t() const;
    operator uint32_t();
    operator int();
    bool operator==(const RenderID &other);
    const bool operator==(const RenderID &other) const;
    bool operator!=(const RenderID &other);
    const bool operator!=(const RenderID &other) const;
    const uint32_t *data() const;
    uint32_t *data();
  };

} // namespace Base
