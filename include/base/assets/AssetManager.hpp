#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace Base
{
  namespace fs = std::filesystem;
  class Sound;
  class AssetManager
  {
  private:
    std::unordered_map<std::string, fs::path> _paths;
    std::unordered_map<std::string, std::shared_ptr<void>> _assets;

    // Sound Settings
    uint64_t _sampleRate = 48000;

  private:
    std::shared_ptr<Sound> LoadSound(const std::filesystem::path &);

  public:
    template <typename T> std::shared_ptr<T> LoadAsset(const fs::path &);
    template <typename T> std::shared_ptr<T> GetAsset(const std::string &);
    template <typename T> void UnloadAsset(const std::string &);

    void AddPath(std::string &name, fs::path path);

    void SetSampleRate(uint64_t sampleRate);
  };
} // namespace Base
