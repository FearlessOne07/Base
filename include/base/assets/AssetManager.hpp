#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/BaseAsset.hpp"
#include "base/audio/AudioStream.hpp"
#include "base/audio/Sound.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
namespace Base
{
  namespace fs = std::filesystem;
  class Scene;
  class AssetManager
  {
    struct AssetSlot
    {
      AssetHandle<void> handle;
      std::shared_ptr<BaseAsset> asset = nullptr;

      AssetSlot() = default;
      AssetSlot(AssetHandle<void> _handle, std::shared_ptr<BaseAsset> _asset) : handle(_handle), asset(_asset)
      {
      }
    };

  private:
    std::unordered_map<std::string, AssetSlot> _globalAssets;
    std::unordered_map<const Scene *, std::unordered_map<std::string, AssetSlot>> _sceneAssets;

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
    template <typename T> AssetHandle<T> GetAsset(const std::string &assetName, const Scene *scene = nullptr)
    {
      if (std::is_base_of_v<BaseAsset, T>)
      {
        std::string name = Base::Strings::ToLower(assetName);
        if (scene)
        {
          if (_sceneAssets.at(scene).find(name) == _sceneAssets.at(scene).end())
          {
            std::stringstream error;
            error << "Scene-local Font '" << name << "' does not exist";
            THROW_BASE_RUNTIME_ERROR(error.str());
          }
          return AssetHandle<T>::Cast(_sceneAssets.at(scene).at(name).handle);
        }
        else
        {
          if (_globalAssets.find(name) == _globalAssets.end())
          {
            std::stringstream error;
            error << "Global asset '" << name << "' does not exist";
            THROW_BASE_RUNTIME_ERROR(error.str());
          }
          return AssetHandle<T>::Cast(_globalAssets.at(name).handle);
        }
      }
    }
  };
} // namespace Base
