#pragma once

#include "base/assets/BaseAsset.hpp"
#include "base/rendering/RenderID.hpp"
#include <filesystem>
#include <memory>

namespace Base
{
  enum class TextureFormat
  {
    RGBA8 = 0,
    RGB8,
    R8,
    RGB16F,
    RGB32F
  };

  enum class TextureFiltering
  {
    Nearest,
    Linear,
  };

  struct TextureSpec
  {
    int Width = 1, Height = 1;
    TextureFormat Format = TextureFormat::RGBA8;
    TextureFiltering MagFilter = TextureFiltering::Linear;
    TextureFiltering MinFilter = TextureFiltering::Linear;
    bool MipMaps = true;
  };

  class Texture : public BaseAsset
  {
  private:
    RenderID _id = 0;
    TextureSpec _spec;

  private:
    Texture(const std::filesystem::path &path, const TextureSpec &spec = {});
    Texture(void *data, const TextureSpec &spec = {});

  public:
    Texture() = default;
    void Bind(int slot = 0) const;
    int GetWidth() const;
    int GetHeight() const;
    RenderID GetRenderID() const;

    static std::shared_ptr<Texture> Create(const std::filesystem::path &path, const TextureSpec &spec = {});
    static std::shared_ptr<Texture> Create(void *data, const TextureSpec &spec = {});

    static void Destroy(std::shared_ptr<Texture> texture);
  };

} // namespace Base
