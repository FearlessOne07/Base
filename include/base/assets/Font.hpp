#pragma once

#include "base/assets/Texture.hpp"
#include "base/util/Type.hpp"
#include <filesystem>

namespace Base
{

  struct MSDFData;
  class Font
  {
    MSDFData *_data;
    Ptr<Texture> _atlas = nullptr;
    Font(const std::filesystem::path &path);

  public:
    ~Font();
    static std::shared_ptr<Font> Create(const std::filesystem::path &);
    static void Destroy(Ptr<Font> &font);
    static Vector2 MeasureText(Ptr<Font> &font, std::string_view string, float fontSize);
    const std::shared_ptr<Texture> GetAtlas();
    const MSDFData *GetMSDFData() const;
  };

} // namespace Base
