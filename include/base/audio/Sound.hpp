#pragma once
#include <array>
#include <cstdint>
#include <vector>

namespace Base
{

  class Sound
  {
  private:
    // Data
    std::vector<int16_t> _data;
    uint8_t _channels = 0;

    // State
    bool _isPlaying = false;
    bool _hasEnded = false;

    uint64_t _currentFrame = 0;
    uint64_t _frameCount = 0;

    // Effects
    float _volume = 1.f;
    float _pan = 0.5;

  public:
    Sound(std::vector<int16_t> data, uint8_t channels, uint64_t frameCount);

    void SetVolume(float volume);
    void SetPan(float pan);
    void Play();

    float GetVolume() const;
    float GetPan() const;
    bool IsPlaying() const;

    // Frames
    std::array<int16_t, 2> GetNextFrame();
    void Reset();
  };
} // namespace Base
