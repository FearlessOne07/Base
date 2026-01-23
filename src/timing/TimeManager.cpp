#include "base/timing/TimeManager.hpp"

namespace Base
{

  float TimeManager::GetDeltaTime()
  {
    auto now = std::chrono::steady_clock::now();

    std::chrono::duration<float> delta = now - _lastFrame;
    _lastFrame = now;

    // FPS tracking
    _frameCount++;

    auto elapsed = now - _fpsTimer;
    if (elapsed >= std::chrono::milliseconds(500))
    {
      _fps = static_cast<int>(_frameCount / std::chrono::duration<float>(elapsed).count());
      _frameCount = 0;
      _fpsTimer = now;
    }

    return delta.count();
  }

  void TimeManager::Init()
  {
    _programStart = std::chrono::steady_clock::now();
    _lastFrame = _programStart;
    _fpsTimer = _programStart;
  }

  double TimeManager::GetTime() const
  {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - _programStart).count();
  }

  int TimeManager::GetFps() const
  {
    return _fps;
  };
} // namespace Base
