#include "base/Component.hpp"
#include "raylib/raylib.h"
#include <bitset>

namespace Base
{

  struct BASEAPI BoundingBoxComponent : public Component
  {
  private:
    std::bitset<3> _typeFlags;

  public:
    enum struct Type : uint8_t
    {
      COLLIDER = 0,
      HURTBOX = 1,
      HITBOX = 2,
    };

    Vector2 size = {0, 0};
    Vector2 positionOffset = {0, 0};
    Vector2 lastPosition = {0, 0};

    bool draw = false;
    bool fill = true;
    Color color = WHITE;
    float nonFillThickness = 3;

    void SetTypeFlag(Type type);
    bool HasTypeFlag(Type type);
  };
} // namespace Base
