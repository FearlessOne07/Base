#include "base/AssetManager.hpp"
#include "raylib.h"
#include <memory>
#include <stdexcept>

namespace Base
{

  template <> void AssetManager::LoadAsset<Texture>(fs::path path)
  {
    std::string name = path.stem().string();
    std::string fullpath = path.string();

    _assets[name] = std::make_shared<Texture>(LoadTexture(fullpath.c_str()));
  }

  template <> void AssetManager::LoadAsset<Music>(fs::path path)
  {
    std::string name = path.stem().string();
    std::string fullpath = path.string();

    _assets[name] = std::make_shared<Music>(LoadMusicStream(fullpath.c_str()));
  }

  template <> void AssetManager::LoadAsset<Sound>(fs::path path)
  {
    std::string name = path.stem().string();
    std::string fullpath = path.string();
    _assets[name] = std::make_shared<Sound>(LoadSound(fullpath.c_str()));
  }

  template <> std::shared_ptr<Texture> AssetManager::GetAsset<Texture>(std::string &name)
  {
    return std::static_pointer_cast<Texture>(_assets.at(name));
  }

  template <> std::shared_ptr<Music> AssetManager::GetAsset<Music>(std::string &name)
  {
    return std::static_pointer_cast<Music>(_assets.at(name));
  }

  template <> std::shared_ptr<Sound> AssetManager::GetAsset<Sound>(std::string &name)
  {
    return std::static_pointer_cast<Sound>(_assets.at(name));
  }

  template <> void AssetManager::UnloadAsset<Texture>(std::string &name)
  {
    if (_assets.find(name) != _assets.end())
    {
      UnloadTexture(*std::static_pointer_cast<Texture>(_assets.at(name)));
      _assets.erase(name);
    }
    else
    {
      throw std::runtime_error("Specified texture does not exist");
    }
  }
  template <> void AssetManager::UnloadAsset<Music>(std::string &name)
  {
    if (_assets.find(name) != _assets.end())
    {
      UnloadMusicStream(*std::static_pointer_cast<Music>(_assets.at(name)));
      _assets.erase(name);
    }
    else
    {
      throw std::runtime_error("Specified music stream does not exist");
    }
  }

  template <> void AssetManager::UnloadAsset<Sound>(std::string &name)
  {
    if (_assets.find(name) != _assets.end())
    {
      UnloadSound(*std::static_pointer_cast<Sound>(_assets.at(name)));
      _assets.erase(name);
    }
    else
    {
      throw std::runtime_error("Specified sound does not exist");
    }
  }
} // namespace Base
