#pragma once
#include <chrono>
namespace Base
{
  class TimeManager
  {
    friend class Game;

  private:
    float _dt = 0;
    double _currentTime = 0;

    std::chrono::time_point<std::chrono::steady_clock> _programStart;
    std::chrono::time_point<std::chrono::steady_clock> _lastFrame;

    int _frameCount = 0;
    int _fps = 0;
    std::chrono::steady_clock::time_point _fpsTimer;

  private:
    float GetDeltaTime();

  public:
    void Init();
    double GetTime() const;
    int GetFps() const;
  };
} // namespace Base
