#include "base/RenderContext.hpp"

namespace Base
{

  Vector2 RenderContext::GetScreenToGame(Vector2 vec) const
  {
    vec.x = (vec.x - marginX) / scale;
    vec.y = (vec.y - marginY) / scale;
    return vec;
  }
} // namespace Base
