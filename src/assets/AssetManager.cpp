#include "base/assets/AssetManager.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/audio/AudioStream.hpp"
#include "base/audio/Sound.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/shaders/Shader.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/textures/Font.hpp"
#include "base/textures/Texture.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include <filesystem>
#include <memory>
#include <miniaudio.h>
#include <raylib.h>
#include <sstream>
#include <string.h>
#include <vector>

namespace Base
{
  void AssetManager::Init()
  {
    // Subscribe to SceneManager events
    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<ScenePoppedSignal>([this](std::shared_ptr<Signal> signal) {
      auto scenePopped = std::static_pointer_cast<ScenePoppedSignal>(signal);
      UnloadScene(scenePopped->scene);
    });

    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> sig) {
      auto scenePushed = std::static_pointer_cast<ScenePushedSignal>(sig);
      SetCurrentScene(scenePushed->scene);
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> sig) {
      auto sceneResumed = std::static_pointer_cast<SceneResumedSignal>(sig);
      SetCurrentScene(sceneResumed->scene);
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
          if (auto it = std::dynamic_pointer_cast<Base::Texture>(asset.asset))
          {
            UnloadTexture(*it->GetRaylibTexture());
          }
          else if (auto it = std::dynamic_pointer_cast<Shader>(asset.asset))
          {
            UnloadShader(*it);
          }
          else if (auto it = std::dynamic_pointer_cast<BaseFont>(asset.asset))
          {
            UnloadFont(*it->GetRaylibFont());
          }
        }
      }
    }
    _sceneAssets.clear();

    // Unload Global Assets
    for (auto &[name, asset] : _globalAssets)
    {
      if (auto it = std::dynamic_pointer_cast<Base::Texture>(asset.asset))
      {
        UnloadTexture(*it->GetRaylibTexture());
      }
      else if (auto it = std::dynamic_pointer_cast<Shader>(asset.asset))
      {
        UnloadShader(*it);
      }
      else if (auto it = std::dynamic_pointer_cast<BaseFont>(asset.asset))
      {
        UnloadFont(*it->GetRaylibFont());
      }
    }

    _globalAssets.clear();
  }

  void AssetManager::SetCurrentScene(const Scene *scene)
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

  void AssetManager::UnloadScene(const Scene *scene)
  {
    // Unload a scene's if it is being popped
    for (auto &[name, asset] : _sceneAssets[scene])
    {
      if (auto it = std::dynamic_pointer_cast<Base::Texture>(asset.asset))
      {
        UnloadTexture(*it->GetRaylibTexture());
      }
      else if (auto it = std::dynamic_pointer_cast<Shader>(asset.asset))
      {
        UnloadShader(*it);
      }
      else if (auto it = std::dynamic_pointer_cast<BaseFont>(asset.asset))
      {
        UnloadFont(*it->GetRaylibFont());
      }
    }
    _sceneAssets.erase(scene);
  }

  std::shared_ptr<Sound> AssetManager::LoadSound(const std::filesystem::path &path)
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

  std::shared_ptr<AudioStream> AssetManager::LoadAudioStream(const std::filesystem::path &path)
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

  template <> AssetHandle<Texture> AssetManager::LoadAsset<Texture>(const fs::path &path, bool global)
  {
    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          Image image = LoadImage(fullpath.c_str()); // Load from disk (CPU)
          Color *pixels = LoadImageColors(image);    // Access raw pixel data

          // Premultiply alpha
          for (int i = 0; i < image.width * image.height; i++)
          {
            if (pixels[i].a == 0)
            {
              pixels[i].r = 0;
              pixels[i].g = 0;
              pixels[i].b = 0;
            }
            else
            {
              pixels[i].r = (pixels[i].r * pixels[i].a) / 255;
              pixels[i].g = (pixels[i].g * pixels[i].a) / 255;
              pixels[i].b = (pixels[i].b * pixels[i].a) / 255;
            }
          }

          // Copy modified pixel data back to the image
          memcpy(image.data, pixels, image.width * image.height * sizeof(Color));

          auto texture = std::make_shared<Texture>(LoadTextureFromImage(image));

          // Cleanup
          UnloadImage(image);
          UnloadImageColors(pixels);

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
            Image image = LoadImage(fullpath.c_str()); // Load from disk (CPU)
            Color *pixels = LoadImageColors(image);    // Access raw pixel data

            // Premultiply alpha
            for (int i = 0; i < image.width * image.height; i++)
            {
              if (pixels[i].a == 0)
              {
                pixels[i].r = 0;
                pixels[i].g = 0;
                pixels[i].b = 0;
              }
              else
              {
                pixels[i].r = (pixels[i].r * pixels[i].a) / 255;
                pixels[i].g = (pixels[i].g * pixels[i].a) / 255;
                pixels[i].b = (pixels[i].b * pixels[i].a) / 255;
              }
            }

            // Copy modified pixel data back to the image
            memcpy(image.data, pixels, image.width * image.height * sizeof(Color));

            auto texture = std::make_shared<Texture>(LoadTextureFromImage(image));

            // Cleanup
            UnloadImage(image);
            UnloadImageColors(pixels);

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

  template <> AssetHandle<BaseShader> AssetManager::LoadAsset<BaseShader>(const fs::path &path, bool global)
  {
    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto shader = std::make_shared<BaseShader>(LoadShader(nullptr, fullpath.c_str()));
          AssetHandle<BaseShader> handle(shader);
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
            auto shader = std::make_shared<BaseShader>(LoadShader(nullptr, fullpath.c_str()));
            AssetHandle<BaseShader> handle(shader);
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
      error << "Cannot find shader file '" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  template <> AssetHandle<Sound> AssetManager::LoadAsset<Sound>(const fs::path &path, bool global)
  {
    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto sound = LoadSound(fullpath.c_str());
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
            auto sound = LoadSound(fullpath.c_str());
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

  template <> AssetHandle<AudioStream> AssetManager::LoadAsset<AudioStream>(const fs::path &path, bool global)
  {
    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto stream = LoadAudioStream(fullpath.c_str());
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
            auto stream = LoadAudioStream(fullpath.c_str());
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

  template <> AssetHandle<BaseFont> AssetManager::LoadAsset<BaseFont>(const fs::path &path, bool global)
  {
    if (fs::exists(Strings::Strip(path.string())))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = Strings::Strip(path.string());

      if (global)
      {
        if (_globalAssets.find(name) == _globalAssets.end())
        {
          auto font = std::make_shared<BaseFont>(LoadFontEx(fullpath.c_str(), 512, nullptr, 0));
          AssetHandle<BaseFont> handle(font);
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
            auto font = std::make_shared<BaseFont>(LoadFontEx(fullpath.c_str(), 512, nullptr, 0));
            AssetHandle<BaseFont> handle(font);
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

  template <> AssetHandle<Texture> AssetManager::GetAsset<Texture>(const std::string &assetName, const Scene *scene)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (scene)
    {
      if (_sceneAssets.at(scene).find(name) == _sceneAssets.at(scene).end())
      {
        std::stringstream error;
        error << "Scene-local Texture '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<Texture>::Cast(_sceneAssets.at(scene).at(name).handle);
    }
    else
    {
      if (_globalAssets.find(name) == _globalAssets.end())
      {
        std::stringstream error;
        error << "Global Texture '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<Texture>::Cast(_globalAssets.at(name).handle);
    }
  }

  template <>
  AssetHandle<BaseShader> AssetManager::GetAsset<BaseShader>(const std::string &assetName, const Scene *scene)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (scene)
    {
      if (_sceneAssets.at(scene).find(name) == _sceneAssets.at(scene).end())
      {
        std::stringstream error;
        error << "Scene-local Shader '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<BaseShader>::Cast(_sceneAssets.at(scene).at(name).handle);
    }
    else
    {
      if (_globalAssets.find(name) == _globalAssets.end())
      {
        std::stringstream error;
        error << "Global Shader '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<BaseShader>::Cast(_globalAssets.at(name).handle);
    }
  }

  template <> AssetHandle<Sound> AssetManager::GetAsset<Sound>(const std::string &assetName, const Scene *scene)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (scene)
    {
      if (_sceneAssets.at(scene).find(name) == _sceneAssets.at(scene).end())
      {
        std::stringstream error;
        error << "Scene-local Sound '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<Sound>::Cast(_sceneAssets.at(scene).at(name).handle);
    }
    else
    {
      if (_globalAssets.find(name) == _globalAssets.end())
      {
        std::stringstream error;
        error << "Global Sound '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<Sound>::Cast(_globalAssets.at(name).handle);
    }
  }

  template <>
  AssetHandle<AudioStream> AssetManager::GetAsset<AudioStream>(const std::string &assetName, const Scene *scene)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (scene)
    {
      if (_sceneAssets.at(scene).find(name) == _sceneAssets.at(scene).end())
      {
        std::stringstream error;
        error << "Scene-local AudioStream '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<AudioStream>::Cast(_sceneAssets.at(scene).at(name).handle);
    }
    else
    {
      if (_globalAssets.find(name) == _globalAssets.end())
      {
        std::stringstream error;
        error << "Global AudioStream '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<AudioStream>::Cast(_globalAssets.at(name).handle);
    }
  }

  template <> AssetHandle<BaseFont> AssetManager::GetAsset<BaseFont>(const std::string &assetName, const Scene *scene)
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
      return AssetHandle<BaseFont>::Cast(_sceneAssets.at(scene).at(name).handle);
    }
    else
    {
      if (_globalAssets.find(name) == _globalAssets.end())
      {
        std::stringstream error;
        error << "Global Font '" << name << "' does not exist";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      return AssetHandle<BaseFont>::Cast(_globalAssets.at(name).handle);
    }
  }
} // namespace Base
