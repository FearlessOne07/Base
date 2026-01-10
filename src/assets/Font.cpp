#include "base/assets/Font.hpp"
#include "core/BitmapRef.hpp"
#include "ext/import-font.h"
#include "internal/utils/MSDFData.hpp"
#include "msdf-atlas-gen/AtlasGenerator.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/Charset.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "msdf-atlas-gen/TightAtlasPacker.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

namespace Base
{

  std::shared_ptr<Font> Font::Create(const std::filesystem::path &path)
  {
    return std::shared_ptr<Font>(new Font(path));
  }

  Font::Font(const std::filesystem::path &path) : _data(new MSDFData)
  {
    msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype();

    if (!ft)
    {
      std::cerr << "failed to init ft\n";
      exit(-1);
    }

    std::string pathName = path.string();
    msdfgen::FontHandle *font = msdfgen::loadFont(ft, pathName.c_str());

    if (!font)
    {
      std::cerr << "failed to load font\n";
      exit(-1);
    }

    struct CharsetRange
    {
      uint32_t Begin, End;
    };

    const CharsetRange charsetRanges[] = {
      {0x0020, 0x00FF},
    };

    msdf_atlas::Charset charset;
    for (CharsetRange range : charsetRanges)
    {
      for (uint32_t c = range.Begin; c <= range.End; c++)
      {
        charset.add(c);
      }
    }

    double fontScale = 1.0;
    _data->FontGeometry = msdf_atlas::FontGeometry(&_data->GlyphGeometry);
    int loaded = _data->FontGeometry.loadCharset(font, fontScale, charset);
    //("Loaded {} out of {}", loaded, charset.size());

    double emSize = 80.0;

    // PackGlpyhs
    msdf_atlas::TightAtlasPacker atlasPacker;
    atlasPacker.setPixelRange(20);
    atlasPacker.setMiterLimit(1.0);
    atlasPacker.setInnerPixelPadding(24);
    atlasPacker.setScale(emSize);
    atlasPacker.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::POWER_OF_TWO_RECTANGLE);
    int remaing = atlasPacker.pack(_data->GlyphGeometry.data(), _data->GlyphGeometry.size());
    assert(remaing == 0);

    int width, height;
    atlasPacker.getDimensions(width, height);
    emSize = atlasPacker.getScale();

    msdf_atlas::GeneratorAttributes attributes;
    attributes.config.overlapSupport = true;
    attributes.scanlinePass = true;

    // Generator config
    msdf_atlas::ImmediateAtlasGenerator< //
      float, 1,                          // pixel type, channel count
      msdf_atlas::sdfGenerator, // which generator to use (sdfGenerator, psdfGenerator, msdfGenerator, mtsdfGenerator)
      msdf_atlas::BitmapAtlasStorage<uint8_t, 1> // where to store results
      >
      generator(width, height);

    generator.setAttributes(attributes);
    generator.setThreadCount(12);
    generator.generate(_data->GlyphGeometry.data(), _data->GlyphGeometry.size());

    msdfgen::BitmapConstRef<uint8_t, 1> bitmap = (msdfgen::BitmapConstRef<uint8_t, 1>)generator.atlasStorage();
    _atlas = Texture::Create( //
      (void *)bitmap.pixels,
      TextureSpec{
        .Width = bitmap.width,
        .Height = bitmap.height,
        .Format = TextureFormat::R8,
        .MipMaps = false,
      } //
    );
    //("Width : {}, Height {}", _atlas->GetWidth(), _atlas->GetHeight());
    msdfgen::destroyFont(font);
    msdfgen::deinitializeFreetype(ft);
  }

  Font::~Font()
  {
    delete _data;
  }

  const std::shared_ptr<Texture> Font::GetAtlas()
  {
    return _atlas;
  }

  const MSDFData *Font::GetMSDFData() const
  {
    return _data;
  }

} // namespace Base
