#include "base/AssetManager.hpp"
#include "internal/Exception/Exception.hpp"
#include <filesystem>
#include <memory>
#include <raylib.h>
#include <sstream>

namespace Base
{
  void AssetManager::AddPath(std::string &name, fs::path path)
  {
    if (_paths.find(name) == _paths.end())
    {
      _paths[name] = std::move(path);
    }
  }

  template <> std::shared_ptr<Texture> AssetManager::LoadAsset<Texture>(const fs::path &path)
  {
    if (fs::exists(path))
    {
      std::string name = path.stem().string();
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

  template <> std::shared_ptr<Music> AssetManager::LoadAsset<Music>(const fs::path &path)
  {
    if (fs::exists(path))
    {
      std::string name = path.stem().string();
      std::string fullpath = path.string();

      if (_assets.find(name) == _assets.end())
      {
        _assets[name] = std::make_shared<Music>(LoadMusicStream(fullpath.c_str()));
        return std::static_pointer_cast<Music>(_assets.at(name));
      }
      else
      {
        std::stringstream error;
        error << "Repeated loading of music stream'" << name << "'";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find music file '" << path.string() << "'";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  template <> std::shared_ptr<Sound> AssetManager::LoadAsset<Sound>(const fs::path &path)
  {
    if (fs::exists(path))
    {
      std::string name = path.stem().string();
      std::string fullpath = path.string();

      if (_assets.find(name) == _assets.end())
      {
        _assets[name] = std::make_shared<Sound>(LoadSound(fullpath.c_str()));
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

  template <> std::shared_ptr<Texture> AssetManager::GetAsset<Texture>(const std::string &name)
  {

    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Texture '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
    return std::static_pointer_cast<Texture>(_assets.at(name));
  }

  template <> std::shared_ptr<Music> AssetManager::GetAsset<Music>(const std::string &name)
  {
    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Music Stream '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
    return std::static_pointer_cast<Music>(_assets.at(name));
  }

  template <> std::shared_ptr<Sound> AssetManager::GetAsset<Sound>(const std::string &name)
  {
    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Sound '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
    return std::static_pointer_cast<Sound>(_assets.at(name));
  }

  template <> void AssetManager::UnloadAsset<Texture>(const std::string &name)
  {
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
  template <> void AssetManager::UnloadAsset<Music>(const std::string &name)
  {
    if (_assets.find(name) != _assets.end())
    {
      UnloadMusicStream(*std::static_pointer_cast<Music>(_assets.at(name)));
      _assets.erase(name);
    }
    else
    {
      std::stringstream error;
      error << "Music Stream '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }

  template <> void AssetManager::UnloadAsset<Sound>(const std::string &name)
  {
    if (_assets.find(name) != _assets.end())
    {
      UnloadSound(*std::static_pointer_cast<Sound>(_assets.at(name)));
      _assets.erase(name);
    }
    else
    {
      std::stringstream error;
      error << "Sound '" << name << "' does not exist";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }
} // namespace Base
