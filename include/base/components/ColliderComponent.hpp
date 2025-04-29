#include "Component.hpp"
#include <raylib.h>
#include <bitset>

namespace Base
{
  struct ColliderComponent : public Component
  {
  public:
    enum struct Shape : uint8_t
    {
      BOX = 0,
      CIRCLE
    };
    enum struct Type : uint8_t
    {
      COLLIDER = 0,
      HURTBOX = 1,
      HITBOX = 2,
    };

  private:
    std::bitset<3> _typeFlags;

  public:
    // Shape
    Shape shape = Shape::BOX;

    // Position
    Vector2 positionOffset = {0, 0};
    Vector2 lastPosition = {0, 0};

    // Box
    Vector2 size = {0, 0};

    // Circle
    float radius = 0;

    // Draw (Temp)
    bool draw = false;
    bool fill = true;
    Color color = WHITE;
    float nonFillThickness = 3;

    void SetTypeFlag(Type type);
    bool HasTypeFlag(Type type);
  };
} // namespace Base
