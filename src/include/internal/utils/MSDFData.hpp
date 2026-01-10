#pragma once
#include "msdf-atlas-gen/FontGeometry.h"
#include <vector>

namespace Base
{
  struct MSDFData
  {
    std::vector<msdf_atlas::GlyphGeometry> GlyphGeometry;
    msdf_atlas::FontGeometry FontGeometry;
  };

} // namespace Base
