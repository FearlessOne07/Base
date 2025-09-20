#pragma once

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
} // namespace Base
