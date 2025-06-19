#pragma once
#include <bitset>

namespace Base
{
  class Pauseable
  {
    std::bitset<8> _pauseMask;

  public:
    Pauseable() = default;
    void SetPauseMask(const std::bitset<8> &mask);
    const std::bitset<8> &GetPauseMask() const;
  };
} // namespace Base
