#include "base/System.hpp"

namespace Base
{
  System::~System()
  {
  }
  void System::Activate()
  {
    _isActive = true;
  }
  void System::Deactivate()
  {
    _isActive = false;
  }
  bool System::IsActive() const
  {
    return _isActive;
  }
} // namespace Base
