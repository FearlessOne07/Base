#include "base/systems/System.hpp"

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

  bool System::IsSuspended()
  {
    return _isSupsended;
  }

  void System::UnSuspend()
  {
    _isSupsended = false;
  }

  void System::Suspend()
  {
    _isSupsended = true;
  }

  void System::Start()
  {
  }

  void System::Stop()
  {
  }

  void System::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
  }
} // namespace Base
