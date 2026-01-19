#include "base/assets/Font.hpp"
#include "base/util/Type.hpp"
#include "internal/utils/MSDFData.hpp"
#include "msdf-atlas-gen/AtlasGenerator.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/Charset.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "msdf-atlas-gen/TightAtlasPacker.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include "msdfgen/core/BitmapRef.hpp"
#include "msdfgen/ext/import-font.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>

namespace Base
{
  static bool DecodeUTF8(const char *&it, const char *end, char32_t &out)
  {
    if (it >= end)
      return false;

    unsigned char c = static_cast<unsigned char>(*it++);

    // 1-byte (ASCII)
    if (c < 0x80)
    {
      out = c;
      return true;
    }

    // 2-byte
    if ((c >> 5) == 0x6 && it < end)
    {
      out = ((c & 0x1F) << 6) | (static_cast<unsigned char>(*it++) & 0x3F);
      return true;
    }

    // 3-byte
    if ((c >> 4) == 0xE && it + 1 < end)
    {
      out = ((c & 0x0F) << 12) | ((static_cast<unsigned char>(*it++) & 0x3F) << 6) |
            (static_cast<unsigned char>(*it++) & 0x3F);
      return true;
    }

    // 4-byte
    if ((c >> 3) == 0x1E && it + 2 < end)
    {
      out = ((c & 0x07) << 18) | ((static_cast<unsigned char>(*it++) & 0x3F) << 12) |
            ((static_cast<unsigned char>(*it++) & 0x3F) << 6) | (static_cast<unsigned char>(*it++) & 0x3F);
      return true;
    }

    // Invalid UTF-8 → replacement character
    out = U'\uFFFD';
    return true;
  }

  // Helper function to convert wstring to UTF-8
  static std::string WStringToUTF8(const std::wstring_view wstr)
  {
    if (wstr.empty())
      return "";

    std::string result;
    result.reserve(wstr.size() * 3); // Reserve approximate space

    for (wchar_t wc : wstr)
    {
      char32_t ch = static_cast<char32_t>(wc);

// Handle UTF-16 surrogate pairs on Windows (where wchar_t is 16-bit)
#ifdef _WIN32
      if (ch >= 0xD800 && ch <= 0xDBFF)
      {
        // High surrogate - skip for now, will be handled with low surrogate
        continue;
      }
      else if (ch >= 0xDC00 && ch <= 0xDFFF)
      {
        // Low surrogate - combine with previous high surrogate
        if (result.size() >= 1)
        {
          // This is simplified; proper implementation would track the high surrogate
          // For now, treat as replacement character
          ch = U'\uFFFD';
        }
      }
#endif

      // Encode to UTF-8
      if (ch < 0x80)
      {
        result.push_back(static_cast<char>(ch));
      }
      else if (ch < 0x800)
      {
        result.push_back(static_cast<char>(0xC0 | (ch >> 6)));
        result.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
      }
      else if (ch < 0x10000)
      {
        result.push_back(static_cast<char>(0xE0 | (ch >> 12)));
        result.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
      }
      else if (ch < 0x110000)
      {
        result.push_back(static_cast<char>(0xF0 | (ch >> 18)));
        result.push_back(static_cast<char>(0x80 | ((ch >> 12) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
      }
    }

    return result;
  }

  std::shared_ptr<Font> Font::Create(const std::filesystem::path &path)
  {
    return std::shared_ptr<Font>(new Font(path));
  }

  void Font::Destroy(Ptr<Font> &font)
  {
    Texture::Destroy(font->_atlas);
  }

  Vector2 Font::MeasureText(const Ptr<Font> &font, const std::string_view text, float fontSize)
  {
    Vector2 result{};

    if (!font->_data || text.empty())
      return result;

    const auto &fontGeom = font->_data->FontGeometry;
    const auto &metrics = fontGeom.getMetrics();

    float scale = fontSize / metrics.emSize;
    float lineHeight = (metrics.ascenderY - metrics.descenderY) * scale;

    // Space advance (for tabs)
    float spaceAdvance = 0.0f;
    if (const auto *space = fontGeom.getGlyph(U' '))
      spaceAdvance = space->getAdvance() * scale;
    else
      spaceAdvance = metrics.emSize * 0.25f * scale;

    float x = 0.0f;
    float maxLineWidth = 0.0f;
    int lineCount = 1;

    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    char32_t prev = 0;

    const char *it = text.data();
    const char *end = it + text.size();

    while (it < end)
    {
      char32_t ch;
      DecodeUTF8(it, end, ch);

      // Newline
      if (ch == U'\n')
      {
        maxLineWidth = std::max(maxLineWidth, x);
        x = 0.0f;
        prev = 0;
        lineCount++;
        continue;
      }

      // Tab = 4 spaces
      if (ch == U'\t')
      {
        x += spaceAdvance * 4.0f;
        prev = 0;
        continue;
      }

      const msdf_atlas::GlyphGeometry *glyph = fontGeom.getGlyph(ch);
      if (!glyph)
        continue;

      // Get advance with kerning using library's helper method
      double advance;
      if (prev && fontGeom.getAdvance(advance, static_cast<msdf_atlas::unicode_t>(prev),
                                      static_cast<msdf_atlas::unicode_t>(ch)))
      {
        x += advance * scale;
      }
      else
      {
        x += glyph->getAdvance() * scale;
      }

      // Vertical bounds (tight)
      double bottom, top, left, right;
      glyph->getQuadPlaneBounds(left, bottom, right, top);
      minY = std::min<float>(minY, bottom * scale);
      maxY = std::max<float>(maxY, top * scale);

      prev = ch;
    }

    maxLineWidth = std::max(maxLineWidth, x);

    result.x = maxLineWidth;

    // Height
    if (lineCount > 1)
      result.y = lineCount * lineHeight;
    else
      result.y = (maxY > minY) ? (maxY - minY) : lineHeight;

    return result;
  }

  // Overload for wstring - converts to UTF-8 and calls the main implementation
  Vector2 Font::MeasureText(const Ptr<Font> &font, const std::wstring_view text, float fontSize)
  {
    std::string utf8Text = WStringToUTF8(text);
    return MeasureText(font, std::string_view(utf8Text), fontSize);
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
