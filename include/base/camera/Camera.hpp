#pragma once
#include "base/util/Type.hpp"
#include "glm/glm.hpp"

namespace Base
{

  class Camera
  {
  public:
    enum struct OriginPoint
    {
      TopLeft = 0,
      Center
    };

  private:
    Vector2 _position{0, 0};
    float _rotation = 0;
    float _zoom = 1.f;
    Vector2 _viewPort{0, 0};

    glm::mat4 _projection;
    glm::mat4 _view;

    OriginPoint _orginPoint = OriginPoint::TopLeft;

  private:
    void RecalculateViewMatrix();

  public:
    Camera(Vector2 viewPort);
    Camera() = default;
    void SetPosition(Vector2 position);
    void SetRotation(float roation);
    void SetZoom(float zoom);
    void SetOrginPoint(OriginPoint point);
    void SetViewPort(Vector2 viewPort);

    const Vector2 GetPosition() const;
    const float GetZoom() const;
    const float GetRotation() const;
    const glm::mat4 GetProjView() const;
  };

} // namespace Base
