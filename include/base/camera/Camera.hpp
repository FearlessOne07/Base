#pragma once
#include "base/rendering/Origin.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  class Camera
  {
  private:
    Vector2 _position{0, 0};
    float _rotation = 0;
    float _zoom = 1.f;
    Vector2 _viewPort{0, 0};

    Mat4 _projection;
    Mat4 _view;
    Mat4 _projView{};
    Mat4 _invProjView{};

    Origin _orginPoint = Origin::TopLeft;
    Vector2 _offset = {0, 0};

  private:
    void RecalculateMatrices();

  public:
    Camera(Vector2 viewPort);
    Camera() = default;
    void SetPosition(Vector2 position);
    void SetRotation(float roation);
    void SetZoom(float zoom);
    void SetOrginPoint(Origin point);
    void SetViewPort(Vector2 viewPort);
    Vector2 GetWorldToScreen(Vector2 world) const;
    Vector2 GetScreenToWorld(Vector2 screen) const;
    float GetScreenToWorld(float distance) const;
    float GetWorldToScreen(float distance) const;

    const Vector2 GetPosition() const;
    const float GetZoom() const;
    const float GetRotation() const;
    const Mat4 GetProjView() const;
  };

} // namespace Base
