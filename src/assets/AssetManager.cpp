#include "base/assets/AssetManager.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/assets/BaseAsset.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include <cassert>
#include <filesystem>
#include <memory>
#include <miniaudio.h>
#include <sstream>
#include <variant>
#include <vector>

namespace fs = std::filesystem;
namespace Base
{
  void AssetManager::Init()
  {
    // Subscribe to SceneManager events
    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<ScenePoppedSignal>([this](std::shared_ptr<Signal> signal) {
      auto scenePopped = std::static_pointer_cast<ScenePoppedSignal>(signal);
      UnloadScene(scenePopped->Scene);
    });

    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> sig) {
      auto scenePushed = std::static_pointer_cast<ScenePushedSignal>(sig);
      SetCurrentScene(scenePushed->Scene);
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> sig) {
      auto sceneResumed = std::static_pointer_cast<SceneResumedSignal>(sig);
      SetCurrentScene(sceneResumed->Scene);
    });
  }

  void AssetManager::Deinit()
  {
    // Unload Scene Assets Still in memory
    if (!_sceneAssets.empty())
    {
      for (auto &[scene, assets] : _sceneAssets)
      {
        for (auto &[name, asset] : assets)
        {
          if (auto it = std::dynamic_pointer_cast<Texture>(asset.asset))
          {
            Texture::Destroy(it);
          }
          else if (auto it = std::dynamic_pointer_cast<Shader>(asset.asset))
          {
            Shader::Delete(it);
          }
          else if (auto it = std::dynamic_pointer_cast<Font>(asset.asset))
          {
            Font::Destroy(it);
          }
        }
      }
    }
    _sceneAssets.clear();

    // Unload Global Assets
    for (auto &[name, asset] : _globalAssets)
    {
      if (auto it = std::dynamic_pointer_cast<Texture>(asset.asset))
      {
        Texture::Destroy(it);
      }
      else if (auto it = std::dynamic_pointer_cast<Shader>(asset.asset))
      {
        Shader::Delete(it);
      }
      else if (auto it = std::dynamic_pointer_cast<Font>(asset.asset))
      {
        Font::Destroy(it);
      }
    }

    _globalAssets.clear();
  }

  void AssetManager::SetCurrentScene(SceneID scene)
  {
    // If scene doesnt exist, it just been pushed
    if (_sceneAssets.find(scene) == _sceneAssets.end())
    {
      // Create its asset list
      _sceneAssets[scene];
    }
    // Update the pointer
    _currentScene = scene;
  }

  void AssetManager::SetAudioSampleRate(uint64_t sampleRate)
  {
    // Called from audio manager to set Audio load-in sample rate
    _sampleRate = sampleRate;
  }

  void AssetManager::UnloadScene(SceneID scene)
  {
    // Unload a scene's if it is being popped
    for (auto &[name, asset] : _sceneAssets[scene])
    {
      if (auto it = std::dynamic_pointer_cast<Texture>(asset.asset))
      {
        Texture::Destroy(it);
      }
      else if (auto it = std::dynamic_pointer_cast<Shader>(asset.asset))
      {
        Shader::Delete(it);
      }
      else if (auto it = std::dynamic_pointer_cast<Font>(asset.asset))
      {
        Font::Destroy(it);
      }
    }
    _sceneAssets.erase(scene);
  }

  std::shared_ptr<Sound> AssetManager::_loadSound(const std::filesystem::path &path)
  {
    ma_result result;
    ma_decoder decoder;
    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, _sampleRate);
    std::string file = path.string();
    result = ma_decoder_init_file(file.c_str(), &config, &decoder);

    uint64_t frameCount = 0;
    std::vector<float> data;
    uint8_t channels = 0;

    if (result != MA_SUCCESS)
    {
      THROW_BASE_RUNTIME_ERROR(                                                                       //
        "Error Reading audio file " + path.string() + "\n Supportted formmats are WAV, MP3 and, FLAC" //
      );
      ma_decoder_uninit(&decoder);
    }

    ma_uint64 maxFrames = decoder.outputSampleRate * 60;
    channels = decoder.outputChannels;
    data.resize(maxFrames * channels);

    result = ma_decoder_read_pcm_frames( //
      &decoder, data.data(), maxFrames,
      (ma_uint64 *)&frameCount //
    );
    ma_decoder_uninit(&decoder);

    if (result != MA_SUCCESS)
    {
      THROW_BASE_RUNTIME_ERROR(                  //
        "Failed to decode sound" + path.string() //
      );
    }
    return std::make_shared<Sound>(data, frameCount, _sampleRate);
  }

  std::shared_ptr<AudioStream> AssetManager::_loadAudioStream(const std::filesystem::path &path)
  {
    ma_result result;
    ma_decoder decoder;
    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 0);
    std::string file = path.string();
    result = ma_decoder_init_file(file.c_str(), &config, &decoder);

    if (result != MA_SUCCESS)
    {
      THROW_BASE_RUNTIME_ERROR(                                                                       //
        "Error Reading audio file " + path.string() + "\n Supportted formmats are WAV, MP3 and, FLAC" //
      );
      ma_decoder_uninit(&decoder);
    }

    return std::make_shared<AudioStream>(decoder, decoder.outputSampleRate, _sampleRate);
  }

  AssetHandle<Texture> AssetManager::LoadTexture(const AssetPath &assetPath, bool global)
  {
    if (!std::holds_alternative<SinglePath>(assetPath))
    {
      THROW_BASE_RUNTIME_ERROR("Invalid Asset Path For Texture Asset\n");
    }

    auto &path = std::get<SinglePath>(assetPath);
    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto texture = Texture::Create(fullpath);

          // Cleanup
          AssetHandle<Texture> handle(texture);
          _globalAssets[name] = {static_cast<AssetHandle<void>>(handle), std::static_pointer_cast<BaseAsset>(texture)};
          return handle;
        }
        else
        {
          std::stringstream error;
          error << "Repeated loading of global texture '" << name << "'";
          THROW_BASE_RUNTIME_ERROR(error.str());
        }
      }
      else
      {
        if (_currentScene)
        {
          if (_sceneAssets.at(_currentScene).find(name) == _sceneAssets.at(_currentScene).end())
          {
            auto texture = Texture::Create(fullpath);
            AssetHandle<Texture> handle(texture);
            _sceneAssets.at(_currentScene)[name] = {
              static_cast<AssetHandle<void>>(handle),
              std::static_pointer_cast<BaseAsset>(texture),
            };
            return handle;
          }
          else
          {
            std::stringstream error;
            error << "Repeated loading of scene-local texture '" << name << "'";
            THROW_BASE_RUNTIME_ERROR(error.str());
          }
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Invalid Scene reference in AssetManager");
        }
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find texture file '" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  AssetHandle<Shader> AssetManager::LoadShader( //
    const AssetPath &path, bool global          //
  )
  {
    if (!std::holds_alternative<ShaderPath>(path))
    {
      THROW_BASE_RUNTIME_ERROR("Invalid Asset Path For Shader Asset\n");
    }

    auto shaderPath = std::get<ShaderPath>(path);
    auto &fragment = std::get<1>(shaderPath);
    auto &vertex = std::get<0>(shaderPath);
    auto &type = std::get<2>(shaderPath);

    bool vert = fs::exists(Strings::Strip(vertex.string()));
    bool frag = fs::exists(Strings::Strip(fragment.string()));
    if (vert || frag)
    {
      std::string name;
      if (frag)
      {
        name = Base::Strings::ToLower(fragment.stem().string());
      }
      else if (vert)
      {
        name = Base::Strings::ToLower(vertex.stem().string());
      }

      std::string fullVertPath = Strings::Strip(vertex.string());
      std::string fullFragPath = Strings::Strip(fragment.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto shader = Shader::Create(fullVertPath, fullFragPath, type);
          AssetHandle<Shader> handle(shader);
          _globalAssets[name] = {static_cast<AssetHandle<void>>(handle), std::static_pointer_cast<BaseAsset>(shader)};
          return handle;
        }
        else
        {
          std::stringstream error;
          error << "Repeated loading of global shader '" << name << "'";
          THROW_BASE_RUNTIME_ERROR(error.str());
        }
      }
      else
      {
        if (_currentScene)
        {
          if (_sceneAssets[_currentScene].find(name) == _sceneAssets.at(_currentScene).end())
          {
            auto shader = Shader::Create(fullVertPath, fullFragPath, type);
            AssetHandle<Shader> handle(shader);
            _sceneAssets[_currentScene][name] = {
              static_cast<AssetHandle<void>>(handle),
              static_pointer_cast<BaseAsset>(shader),
            };
            return handle;
          }
          else
          {
            std::stringstream error;
            error << "Repeated loading of scene-local shader '" << name << "'";
            THROW_BASE_RUNTIME_ERROR(error.str());
          }
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Invalid Scene reference in AssetManager");
        }
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find Vertex shader file '" << vertex.string() << "' or Fragment Shader file '"
            << fragment.string() << "\n";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  AssetHandle<Sound> AssetManager::LoadSound(const AssetPath &assetPath, bool global)
  {
    if (!std::holds_alternative<SinglePath>(assetPath))
    {
      THROW_BASE_RUNTIME_ERROR("Invalid Asset Path For Shader Asset\n");
    }
    auto &path = std::get<SinglePath>(assetPath);

    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto sound = _loadSound(fullpath.c_str());
          AssetHandle<Sound> handle(sound);
          _globalAssets[name] = {static_cast<AssetHandle<void>>(handle), std::static_pointer_cast<BaseAsset>(sound)};
          return handle;
        }
        else
        {
          std::stringstream error;
          error << "Repeated loading of global sound '" << name << "'";
          THROW_BASE_RUNTIME_ERROR(error.str());
        }
      }
      else
      {
        if (_currentScene)
        {
          if (_sceneAssets[_currentScene].find(name) == _sceneAssets.at(_currentScene).end())
          {
            auto sound = _loadSound(fullpath.c_str());
            AssetHandle<Sound> handle(sound);
            _sceneAssets[_currentScene][name] = {
              static_cast<AssetHandle<void>>(handle),
              static_pointer_cast<BaseAsset>(sound),
            };
            return handle;
          }
          else
          {
            std::stringstream error;
            error << "Repeated loading of scene-local sound '" << name << "'";
            THROW_BASE_RUNTIME_ERROR(error.str());
          }
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Invalid Scene reference in AssetManager");
        }
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find sound file '" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  AssetHandle<AudioStream> AssetManager::LoadAudioStream(const AssetPath &assetPath, bool global)
  {
    if (!std::holds_alternative<SinglePath>(assetPath))
    {
      THROW_BASE_RUNTIME_ERROR("Invalid Asset Path For Shader Asset\n");
    }

    auto &path = std::get<SinglePath>(assetPath);

    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto stream = _loadAudioStream(fullpath.c_str());
          AssetHandle<AudioStream> handle(stream);
          _globalAssets[name] = {static_cast<AssetHandle<void>>(handle), std::static_pointer_cast<BaseAsset>(stream)};
          return handle;
        }
        else
        {
          std::stringstream error;
          error << "Repeated loading of global audio-stream'" << name << "'";
          THROW_BASE_RUNTIME_ERROR(error.str());
        }
      }
      else
      {
        if (_currentScene)
        {
          if (_sceneAssets[_currentScene].find(name) == _sceneAssets.at(_currentScene).end())
          {
            auto stream = _loadAudioStream(fullpath.c_str());
            AssetHandle<AudioStream> handle(stream);
            _sceneAssets[_currentScene][name] = {
              static_cast<AssetHandle<void>>(handle),
              static_pointer_cast<BaseAsset>(stream),
            };
            return handle;
          }
          else
          {
            std::stringstream error;
            error << "Repeated loading of scene-local audio-stream '" << name << "'";
            THROW_BASE_RUNTIME_ERROR(error.str());
          }
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Invalid Scene reference in AssetManager");
        }
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find audio-stream file '" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  AssetHandle<Font> AssetManager::LoadFont(const AssetPath &assetPath, bool global)
  {
    if (!std::holds_alternative<SinglePath>(assetPath))
    {
      THROW_BASE_RUNTIME_ERROR("Invalid Asset Path For Font Asset\n");
    }

    auto &path = std::get<SinglePath>(assetPath);
    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto font = Font::Create(fullpath);
          AssetHandle<Font> handle(font);
          _globalAssets[name] = {static_cast<AssetHandle<void>>(handle), std::static_pointer_cast<BaseAsset>(font)};
          return handle;
        }
        else
        {
          std::stringstream error;
          error << "Repeated loading of global font '" << name << "'";
          THROW_BASE_RUNTIME_ERROR(error.str());
        }
      }
      else
      {
        if (_currentScene)
        {
          if (_sceneAssets[_currentScene].find(name) == _sceneAssets.at(_currentScene).end())
          {
            auto font = Font::Create(fullpath.c_str());
            AssetHandle<Font> handle(font);
            _sceneAssets[_currentScene][name] = {
              static_cast<AssetHandle<void>>(handle),
              static_pointer_cast<BaseAsset>(font),
            };
            return handle;
          }
          else
          {
            std::stringstream error;
            error << "Repeated loading of scene-local font '" << name << "'";
            THROW_BASE_RUNTIME_ERROR(error.str());
          }
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Invalid Scene reference in AssetManager");
        }
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find font file '" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }
} // namespace Base
