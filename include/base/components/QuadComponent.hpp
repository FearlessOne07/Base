#include "base/components/Component.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  class QuadComponent : public Component
  {
  private:
    Color _color = {255, 255, 255, 255};
    Vector2 _size = {100, 100};

  public:
    QuadComponent(Color color, Vector2 size);

    Vector2 GetSize() const;
    Color GetColor() const;
  };
} // namespace Base
