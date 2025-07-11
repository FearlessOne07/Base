#include "internal/audio/SoundInstance.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>

namespace Base
{

  SoundInstance::SoundInstance(std::shared_ptr<Sound> sound) : _sound(sound)
  {
  }

  void SoundInstance::SetVolume(float volume)
  {
    _volume = volume;
    _volume = std::clamp<float>(_volume, 0.f, 1.f);
  }

  void SoundInstance::SetPan(float pan)
  {
    _pan = pan;
    _pan = std::clamp<float>(_pan, -1.f, 1.f);
  }

  void SoundInstance::Play()
  {
    _isPlaying = true;
  }

  float SoundInstance::GetVolume() const
  {
    return _volume;
  }
  float SoundInstance::GetPan() const
  {
    return _pan;
  }

  bool SoundInstance::IsPlaying() const
  {
    return _isPlaying;
  }

  std::array<float, 2> SoundInstance::GetNextFrame()
  {
    std::array<float, 2> frame;

    // If sound hasn;t ended
    if (_currentFrame < _sound->GetFrameCount())
    {
      // Get next frame from sound
      frame = _sound->GetFrame(_currentFrame);
      // Ppaly pan / volume
      float angle = _pan * (PI / 2.f);
      float leftPan = std::cos(angle);
      float rightPan = std::sin(angle);

      frame[0] = frame[0] * _volume * leftPan;
      frame[1] = frame[1] * _volume * rightPan;
    }
    else
    {
      // Else return silence
      frame[0] = 0;
      frame[1] = 0;

      // Mark instance as done playing
      _isPlaying = false;
    }

    /// Advance frame
    _currentFrame++;
    return frame;
  }

} // namespace Base
