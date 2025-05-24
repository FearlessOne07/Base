#include "base/audio/Sound.hpp"
#include <algorithm>
namespace Base
{

  Sound::Sound(std::vector<int16_t> data, uint8_t channels, uint64_t frameCount)
  {
  }

  void Sound::SetVolume(float volume)
  {
    _volume = volume;
    _volume = std::clamp<float>(_volume, 0.f, 1.f);
  }

  void Sound::SetPan(float pan)
  {
    _pan = pan;
    _pan = std::clamp<float>(_pan, -1.f, 1.f);
  }

  void Sound::Play()
  {
    _isPlaying = true;
  }

  float Sound::GetVolume() const
  {
    return _volume;
  }
  float Sound::GetPan() const
  {
    return _pan;
  }
  bool Sound::IsPlaying() const
  {
    return _isPlaying;
  }

  std::array<int16_t, 2> Sound::GetNextFrame()
  {
    std::array<int16_t, 2> frame;
    if (_currentFrame < _frameCount)
    {

      float angle = _pan * M_PI_2; // π/2 = 90°, convert pan to radians
      float leftPan = std::cos(angle);
      float rightPan = std::sin(angle);

      frame[0] = _data[_currentFrame * 2] * _volume * leftPan;
      frame[1] = _data[_currentFrame * 2 + 1] * _volume * rightPan;
    }
    else
    {
      frame[0] = 0;
      frame[1] = 0;
      _isPlaying = false;
    }

    _currentFrame++;
    return frame;
  }

  void Sound::Reset()
  {
    _currentFrame = 0;
  }
} // namespace Base
