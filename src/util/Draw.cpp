#include "base/util/Draw.hpp"
#include "raylib.h"
namespace Base
{
  void DrawRectangleBase(Rectangle rec, Vector2 origin, float rotation, Color color)
  {
    DrawRectanglePro( //
      rec, origin, rotation,
      {
        static_cast<unsigned char>((color.r * color.a) / 255),
        static_cast<unsigned char>((color.g * color.a) / 255),
        static_cast<unsigned char>((color.b * color.a) / 255),
        color.a,
      } //
    );
  }

  void DrawTextBase(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint)
  {
    EndBlendMode();
    BeginBlendMode(BLEND_ALPHA);
    DrawTextEx(font, text, position, fontSize, spacing, tint);
    EndBlendMode();
    BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
  }
} // namespace Base
