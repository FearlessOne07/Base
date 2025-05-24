#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI  // Excludes GDI (Graphics Device Interface)
#define NOUSER // Excludes USER APIs
#endif         // WIN32

#include "base/assets/AssetManager.hpp"
#include "base/audio/Sound.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include <filesystem>
#include <memory>
#include <miniaudio.h>
#include <raylib.h>
#include <sstream>
#include <vector>

namespace Base
{
  void AssetManager::AddPath(std::string &name, fs::path path)
  {
    if (_paths.find(name) == _paths.end())
    {
      _paths[name] = std::move(path);
    }
  }

  std::shared_ptr<Sound> AssetManager::LoadSound(const std::filesystem::path &path)
  {
    ma_result result;
    ma_decoder decoder;
    ma_decoder_config config = ma_decoder_config_init(ma_format_s16, 2, 44100);
    std::string file = path.string();
    result = ma_decoder_init_file(file.c_str(), &config, &decoder);

    uint64_t frameCount = 0;
    std::vector<int16_t> data;
    uint8_t channels = 0;

    if (result != MA_SUCCESS)
    {
      THROW_BASE_RUNTIME_ERROR("Error Reading audio file " + path.string());
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
      THROW_BASE_RUNTIME_ERROR("Failed to decode sound" + path.string());
    }

    return std::make_shared<Sound>(data, channels, frameCount);
  }

  template <> std::shared_ptr<Texture> AssetManager::LoadAsset<Texture>(const fs::path &path)
  {
    if (fs::exists(path))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = path.string();
      if (_assets.find(name) == _assets.end())
      {
        _assets[name] = std::make_shared<Texture>(LoadTexture(fullpath.c_str()));
        return std::static_pointer_cast<Texture>(_assets.at(name));
      }
      else
      {
        std::stringstream error;
        error << "Repeated loading of texture '" << name << "'";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find teture file '" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  template <> std::shared_ptr<Sound> AssetManager::LoadAsset<Sound>(const fs::path &path)
  {
    if (fs::exists(path))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = path.string();

      if (_assets.find(name) == _assets.end())
      {
        _assets[name] = LoadSound(fullpath.c_str());
        return std::static_pointer_cast<Sound>(_assets.at(name));
      }
      else
      {
        std::stringstream error;
        error << "Repeated loading of sound '" << name << "'";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find sound file'" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  template <> std::shared_ptr<Font> AssetManager::LoadAsset<Font>(const fs::path &path)
  {
    if (fs::exists(path))
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      std::string fullpath = path.string();

      if (_assets.find(name) == _assets.end())
      {
        _assets[name] = std::make_shared<Font>(LoadFontEx(fullpath.c_str(), 512, nullptr, 0));
        return std::static_pointer_cast<Font>(_assets.at(name));
      }
      else
      {
        std::stringstream error;
        error << "Repeated loading of font '" << name << "'";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find font file'" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  template <> std::shared_ptr<Texture> AssetManager::GetAsset<Texture>(const std::string &assetName)
  {

    std::string name = Base::Strings::ToLower(assetName);
    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Texture '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
    return std::static_pointer_cast<Texture>(_assets.at(name));
  }

  template <> std::shared_ptr<Sound> AssetManager::GetAsset<Sound>(const std::string &assetName)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Sound '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
    return std::static_pointer_cast<Sound>(_assets.at(name));
  }

  template <> std::shared_ptr<Font> AssetManager::GetAsset<Font>(const std::string &assetName)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Font '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
    return std::static_pointer_cast<Font>(_assets.at(name));
  }

  template <> void AssetManager::UnloadAsset<Texture>(const std::string &assetName)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (_assets.find(name) != _assets.end())
    {
      UnloadTexture(*std::static_pointer_cast<Texture>(_assets.at(name)));
      _assets.erase(name);
    }
    else
    {
      std::stringstream error;
      error << "Texture '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  template <> void AssetManager::UnloadAsset<Sound>(const std::string &assetName)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (_assets.find(name) != _assets.end())
    {
      _assets.erase(name);
    }
    else
    {
      std::stringstream error;
      error << "Sound '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  template <> void AssetManager::UnloadAsset<Font>(const std::string &assetName)
  {
    std::string name = Base::Strings::ToLower(assetName);
    if (_assets.find(name) != _assets.end())
    {
      UnloadFont(*std::static_pointer_cast<Font>(_assets.at(name)));
      _assets.erase(name);
    }
    else
    {
      std::stringstream error;
      error << "Font '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }
} // namespace Base
