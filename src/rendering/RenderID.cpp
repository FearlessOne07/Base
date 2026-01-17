
#include "base/rendering/RenderID.hpp"

namespace Base
{

  RenderID::operator bool()
  {
    return _id != 0;
  }

  RenderID::operator uint32_t() const
  {
    return _id;
  }

  RenderID::operator uint32_t()
  {
    return _id;
  }

  bool RenderID::operator!=(const RenderID &other)
  {
    return _id != other._id;
  }

  bool RenderID::operator==(const RenderID &other)
  {
    return _id == other._id;
  }

  const bool RenderID::operator==(const RenderID &other) const
  {
    return _id == other._id;
  }

  const uint32_t *RenderID::data() const
  {
    return &_id;
  }

  uint32_t *RenderID::data()
  {
    return &_id;
  }

  RenderID::RenderID(uint32_t id) : _id(id)
  {
  }

  RenderID::RenderID()
  {
    _id = 0;
  }

} // namespace Base
