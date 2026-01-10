#pragma once
#include "base/rendering/Origin.hpp"
#include "base/rendering/Renderable.hpp"
#include "base/util/Type.hpp"

namespace Base
{

  class Rectangle : public Renderable
  {
  private:
    Vector2 _size = {0, 0};
    Vector2 _position = {0, 0};
    Origin _origin = Origin::TopLeft;

  public:
    Rectangle();
    Rectangle(Vector2 size, Vector2 position, Origin origin = Origin::TopLeft);
    Rectangle(Vector2 size, Vector2 position, const Material &material, Origin origin = Origin::TopLeft);
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
    Origin GetOrigin() const;
  };

} // namespace Base
