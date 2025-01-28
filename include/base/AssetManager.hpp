#pragma once
#include "base/Exports.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace Base
{
  namespace fs = std::filesystem;
  class BASEAPI AssetManager
  {
  private:
    std::unordered_map<std::string, std::shared_ptr<void>> _assets;

  public:
    template <typename T> void LoadAsset(fs::path)
    {
      static_assert(std::is_same_v<T, void>, "This asset type is not supported!");
    }

    template <typename T> std::shared_ptr<T> GetAsset(std::string &)
    {
      static_assert(std::is_same_v<T, void>, "This asset type is not supported!");
    }

    template <typename T> void UnloadAsset(std::string &)
    {
      static_assert(std::is_same_v<T, void>, "This asset type is not supported!");
    }
  };
} // namespace Base
