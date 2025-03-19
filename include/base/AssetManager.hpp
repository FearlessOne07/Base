#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace Base
{
  namespace fs = std::filesystem;
  class AssetManager
  {
  private:
    std::unordered_map<std::string, fs::path> _paths;
    std::unordered_map<std::string, std::shared_ptr<void>> _assets;

  public:
    template <typename T> std::shared_ptr<T> LoadAsset(const fs::path &);
    template <typename T> std::shared_ptr<T> GetAsset(const std::string &);
    template <typename T> void UnloadAsset(const std::string &);

    void AddPath(std::string &name, fs::path path);
  };
} // namespace Base
