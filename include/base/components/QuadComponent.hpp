#include "base/components/Component.hpp"
#include "raylib.h"

namespace Base
{
  class QuadComponent : public Component
  {
  private:
    Color _color = WHITE;
    Vector2 _size = {100, 100};

  public:
    QuadComponent(Color color, Vector2 size);

    Vector2 GetSize() const;
    Color GetColor() const;
  };
} // namespace Base
