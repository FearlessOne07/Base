#include "Component.hpp"
#include "base/util/Type.hpp"
#include <bitset>

namespace Base
{
  class ColliderComponent : public Component
  {
  public:
    enum struct Shape : uint8_t
    {
      Box = 0,
      Circle
    };

  private:
    // To Be Implemented
    std::bitset<8> _layer;
    std::bitset<8> _mask;

  public:
    // Shape
    Shape shape = Shape::Box;

    // Position
    Vector2 positionOffset = {0, 0};
    Vector2 lastPosition = {0, 0};

    // Box
    Vector2 size = {0, 0};

    // Circle
    float radius = 0;
  };
} // namespace Base
