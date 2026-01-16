#pragma once

#include <array>
#include <filesystem>
#include <variant>
namespace Base
{
  class BaseAsset
  {
  public:
    virtual ~BaseAsset();
  };

  enum class AssetType
  {
    Texture = 0,
    Sound,
    AudioStream,
    Font,
    Shader
  };

  using SinglePath = std::filesystem::path;
  using DoublePath = std::array<std::filesystem::path, 2>;
  using AssetPath = std::variant<SinglePath, DoublePath>;
} // namespace Base
