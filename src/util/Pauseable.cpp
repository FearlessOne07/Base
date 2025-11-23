#include "base/util/Pauseable.hpp"
#include <bitset>

namespace Base
{
  void Pauseable::SetPauseMask(const std::bitset<8> &mask)
  {
    _pauseMask = mask;
  }

  const std::bitset<8> &Pauseable::GetPauseMask() const
  {
    return _pauseMask;
  }
} // namespace Base
