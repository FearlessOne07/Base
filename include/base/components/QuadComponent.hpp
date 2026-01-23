#include "base/components/Component.hpp"
#include "base/rendering/Quad.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  class QuadComponent : public Component
  {
  private:
    Color _color = {255, 255, 255, 255};
    Rectangle _rectangle;

  public:
    QuadComponent(const Rectangle &rectangle, Color color);
    const Rectangle &GetRectangle() const;
    const Color &GetColor() const;
  };
} // namespace Base
