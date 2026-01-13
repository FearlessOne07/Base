#pragma once
#include "base/rendering/Material.hpp"
#include "base/rendering/Renderable.hpp"
#include "base/util/Type.hpp"
namespace Base
{
  class Circle : public Renderable
  {
  private:
    Vector2 _position = {0, 0};
    float _radius = 0;
    float _thickness = 0;

  public:
    Circle(Vector2 position, float radius);
    Circle(Vector2 position, float radius, const Material &material);

    Circle(Vector2 position, float radius, float thickness);
    Circle(Vector2 position, float radius, float thickness, const Material &material);

    float GetRadius() const;
    Vector2 GetPosition() const;
  };
} // namespace Base
