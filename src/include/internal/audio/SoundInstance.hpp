#pragma once

#include "base/audio/Sound.hpp"
#include <cstdint>
#include <memory>
namespace Base
{
  class SoundInstance
  {
    std::shared_ptr<Sound> _sound;
    uint64_t _currentFrame = 0;

    // Effects
    float _volume = 1.f;
    float _pan = 0.5;

    // State
    bool _isPlaying = false;

  public:
    SoundInstance(std::shared_ptr<Sound> sound);
    std::array<int16_t, 2> GetNextFrame();

    void SetVolume(float volume);
    void SetPan(float pan);
    void Play();

    float GetVolume() const;
    float GetPan() const;
    bool IsPlaying() const;
  };
}; // namespace Base
