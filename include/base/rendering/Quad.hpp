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
    Rectangle(Vector2 size, Origin origin = Origin::TopLeft);
    Rectangle(Vector2 size, const Material &material, Origin origin = Origin::TopLeft);
    Rectangle(Vector2 position, Vector2 size, Origin origin = Origin::TopLeft);
    Rectangle(Vector2 position, Vector2 size, const Material &material, Origin origin = Origin::TopLeft);
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
    Origin GetOrigin() const;
    void SetPosition(Vector2 size);
    void SetSize(Vector2 size);
  };

} // namespace Base
