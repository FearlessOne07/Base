#pragma once
#include "base/rendering/Quad.hpp"
#include "base/util/Circle.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  bool CheckCollisionPointRec(Vector2 point, const Rectangle &rec);

  bool CheckCollisionPointCircle(Vector2 point, const Circle &circle);
  bool CheckCollisionRecs(const Rectangle &rec1, const Rectangle &rec2);

  bool CheckCollisionCircles(const Circle &circle1, const Circle &circle2);
  bool CheckCollisionCircleRec(const Circle &circle, const Rectangle &rec);
} // namespace Base
