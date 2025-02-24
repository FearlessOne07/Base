#include "base/AssetManager.hpp"
#include "raylib.h"
#include <filesystem>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace Base
{
  void AssetManager::AddPath(std::string name, fs::path path)
  {
    if (_paths.find(name) == _paths.end())
    {
      _paths[name] = path;
    }
  }

  template <> BASEAPI std::shared_ptr<Texture> AssetManager::LoadAsset<Texture>(fs::path path)
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
        throw std::runtime_error(error.str());
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find teture file '" << path.string() << "'";
      throw std::runtime_error(error.str());
    }
  }

  template <> BASEAPI std::shared_ptr<Music> AssetManager::LoadAsset<Music>(fs::path path)
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
        throw std::runtime_error(error.str());
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find music file '" << path.string() << "'";
      throw std::runtime_error(error.str());
    }
  }

  template <> BASEAPI std::shared_ptr<Sound> AssetManager::LoadAsset<Sound>(fs::path path)
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
        throw std::runtime_error(error.str());
      }
    }
    else
    {
      std::stringstream error;
      error << "Cannot find sound file'" << path.string() << "'";
      throw std::runtime_error(error.str());
    }
  }

  template <> BASEAPI std::shared_ptr<Texture> AssetManager::GetAsset<Texture>(const std::string &name)
  {

    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Texture '" << name << "' does not exist";
      throw std::runtime_error(error.str());
    }
    return std::static_pointer_cast<Texture>(_assets.at(name));
  }

  template <> BASEAPI std::shared_ptr<Music> AssetManager::GetAsset<Music>(const std::string &name)
  {
    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Music Stream '" << name << "' does not exist";
      throw std::runtime_error(error.str());
    }
    return std::static_pointer_cast<Music>(_assets.at(name));
  }

  template <> BASEAPI std::shared_ptr<Sound> AssetManager::GetAsset<Sound>(const std::string &name)
  {
    if (_assets.find(name) == _assets.end())
    {
      std::stringstream error;
      error << "Sound '" << name << "' does not exist";
      throw std::runtime_error(error.str());
    }
    return std::static_pointer_cast<Sound>(_assets.at(name));
  }

  template <> BASEAPI void AssetManager::UnloadAsset<Texture>(const std::string &name)
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
      throw std::runtime_error(error.str());
    }
  }
  template <> BASEAPI void AssetManager::UnloadAsset<Music>(const std::string &name)
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
      throw std::runtime_error(error.str());
    }
  }

  template <> BASEAPI void AssetManager::UnloadAsset<Sound>(const std::string &name)
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
      throw std::runtime_error(error.str());
    }
  }
} // namespace Base
