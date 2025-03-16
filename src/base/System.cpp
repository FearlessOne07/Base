#include "base/System.hpp"

namespace Base
{
  void System::Activate()
  {
    Start();
    _isActive = true;
  }
  void System::Deactivate()
  {
    Stop();
    _isActive = false;
  }
  bool System::IsActive() const
  {
    return _isActive;
  }
} // namespace Base
