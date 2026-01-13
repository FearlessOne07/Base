#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/BaseAsset.hpp"
#include "base/assets/Font.hpp"
#include "base/assets/Texture.hpp"
#include "base/audio/AudioStream.hpp"
#include "base/audio/Sound.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/shaders/Shader.hpp"
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
    std::unordered_map<SceneID, std::unordered_map<std::string, AssetSlot>> _sceneAssets;

    SceneID _currentScene;

    // Sound Settings
    uint64_t _sampleRate = 48000;

  private:
    std::shared_ptr<Sound> _loadSound(const std::filesystem::path &);
    std::shared_ptr<AudioStream> _loadAudioStream(const std::filesystem::path &);
    void UnloadScene(SceneID);
    void SetCurrentScene(SceneID);

  public:
    void Init();
    void Deinit();
    void SetAudioSampleRate(uint64_t sampleRate);

    template <typename T>
      requires(std::is_base_of_v<BaseAsset, T>)
    AssetHandle<T> GetAsset(const std::string &assetName, SceneID scene = SceneID())
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
    AssetHandle<Texture> LoadTexture(const fs::path &path, bool global);
    AssetHandle<Shader> LoadShader(const fs::path &vertex, const fs::path &fragment, GeometryType type, bool global);
    AssetHandle<Sound> LoadSound(const fs::path &path, bool global);
    AssetHandle<AudioStream> LoadAudioStream(const fs::path &path, bool global);
    AssetHandle<Font> LoadFont(const fs::path &path, bool global);
  };
} // namespace Base
