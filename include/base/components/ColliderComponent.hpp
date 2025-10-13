#include "Component.hpp"
#include <bitset>
#include <raylib.h>

namespace Base
{
  class ColliderComponent : public Component
  {
  public:
    enum struct Shape : uint8_t
    {
      BOX = 0,
      CIRCLE
    };

  private:
    // To Be Implemented
    std::bitset<8> _layer;
    std::bitset<8> _mask;

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
  };
} // namespace Base
