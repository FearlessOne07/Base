#pragma once

#include "base/rendering/GeometryType.hpp"
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
  using ShaderPath = std::tuple<std::filesystem::path, std::filesystem::path, GeometryType>;
  using AssetPath = std::variant<SinglePath, ShaderPath>;
} // namespace Base
