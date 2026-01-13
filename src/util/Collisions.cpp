
#include "internal/utils/Collisions.hpp"

namespace Base
{
  bool CheckCollisionPointRec(Vector2 point, const Rectangle &rec)
  {
    bool collision = false;

    if ((point.x >= rec.GetPosition().x) && (point.x < (rec.GetPosition().x + rec.GetSize().x)) &&
        (point.y >= rec.GetPosition().y) && (point.y < (rec.GetPosition().y + rec.GetSize().y)))
      collision = true;

    return collision;
  }

  // Check if point is inside circle
  bool CheckCollisionPointCircle(Vector2 point, const Circle &circle)
  {
    bool collision = false;

    float distanceSquared = (point.x - circle.GetPosition().x) * (point.x - circle.GetPosition().x) +
                            (point.y - circle.GetPosition().y) * (point.y - circle.GetPosition().y);

    if (distanceSquared <= circle.GetRadius() * circle.GetRadius())
    {
      collision = true;
    }

    return collision;
  }

  // Check collision between two rectangles
  bool CheckCollisionRecs(const Rectangle &rec1, const Rectangle &rec2)
  {
    bool collision = false;

    if ((rec1.GetPosition().x < (rec2.GetPosition().x + rec2.GetSize().x) &&
         (rec1.GetPosition().x + rec1.GetSize().x) > rec2.GetPosition().x) &&
        (rec1.GetPosition().y < (rec2.GetPosition().y + rec2.GetSize().y) &&
         (rec1.GetPosition().y + rec1.GetSize().y) > rec2.GetPosition().y))
      collision = true;

    return collision;
  }

  // Check collision between two circles
  bool CheckCollisionCircles(const Circle &circle1, const Circle &circle2)
  {
    bool collision = false;

    float dx = circle2.GetPosition().x - circle1.GetPosition().x; // X distance between centers
    float dy = circle2.GetPosition().y - circle1.GetPosition().y; // Y distance between centers

    float distanceSquared = dx * dx + dy * dy; // Distance between centers squared
    float radiusSum = circle2.GetRadius() + circle1.GetRadius();

    collision = (distanceSquared <= (radiusSum * radiusSum));

    return collision;
  }

  // Check collision between circle and rectangle
  // NOTE: Reviewed version to take into account corner limit case
  bool CheckCollisionCircleRec(const Circle &circle, const Rectangle &rec)
  {
    bool collision = false;

    float recCenterX = rec.GetPosition().x + rec.GetSize().x / 2.0f;
    float recCenterY = rec.GetPosition().y + rec.GetSize().y / 2.0f;

    float dx = fabsf(circle.GetPosition().x - recCenterX);
    float dy = fabsf(circle.GetPosition().y - recCenterY);

    if (dx > (rec.GetSize().x / 2.0f + circle.GetRadius()))
    {
      return false;
    }
    if (dy > (rec.GetSize().y / 2.0f + circle.GetRadius()))
    {
      return false;
    }

    if (dx <= (rec.GetSize().x / 2.0f))
    {
      return true;
    }
    if (dy <= (rec.GetSize().y / 2.0f))
    {
      return true;
    }

    float cornerDistanceSq = (dx - rec.GetSize().x / 2.0f) * (dx - rec.GetSize().x / 2.0f) +
                             (dy - rec.GetSize().y / 2.0f) * (dy - rec.GetSize().y / 2.0f);

    collision = (cornerDistanceSq <= (circle.GetRadius() * circle.GetRadius()));

    return collision;
  }
} // namespace Base
