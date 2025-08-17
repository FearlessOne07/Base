#include "base/components/TransitionComponent.hpp"

namespace Base
{
  void TransitionComponent::SetBlockIndex(size_t index)
  {
    _blockIndex = index;
  }

  size_t TransitionComponent::GetBlockIndex() const
  {
    return _blockIndex;
  }

} // namespace Base
