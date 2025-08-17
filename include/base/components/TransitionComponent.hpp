#pragma once
#include "base/components/Component.hpp"
#include <cstddef>
namespace Base
{
  class TransitionComponent : public Component
  {
  private:
    size_t _blockIndex;

  public:
    void SetBlockIndex(size_t index);
    size_t GetBlockIndex() const;
  };
} // namespace Base
