#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/BaseAsset.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
namespace Base
{
  namespace fs = std::filesystem;
  class Sound;
  class AudioStream;
  class Scene;
  class AssetManager
  {
    struct Asset
    {
      AssetHandle<void> handle;
      std::shared_ptr<BaseAsset> asset = nullptr;

      Asset() = default;
      Asset(AssetHandle<void> _handle, std::shared_ptr<BaseAsset> _asset) : handle(_handle), asset(_asset)
      {
      }
    };

  private:
    std::unordered_map<std::string, Asset> _globalAssets;
    std::unordered_map<const Scene *, std::unordered_map<std::string, Asset>> _sceneAssets;

    const Scene *_currentScene = nullptr;

    // Sound Settings
    uint64_t _sampleRate = 48000;

  private:
    std::shared_ptr<Sound> LoadSound(const std::filesystem::path &);
    std::shared_ptr<AudioStream> LoadAudioStream(const std::filesystem::path &);
    void UnloadScene(const Scene *);
    void SetCurrentScene(const Scene *);

  public:
    void Init();
    void Deinit();
    void SetAudioSampleRate(uint64_t sampleRate);
    template <typename T> AssetHandle<T> LoadAsset(const fs::path &, bool global = true);
    template <typename T> AssetHandle<T> GetAsset(const std::string &, const Scene *scene = nullptr);
  };
} // namespace Base
