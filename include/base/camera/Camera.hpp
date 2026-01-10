#pragma once
#include "Camera.hpp"
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
    glm::vec2 _position{0, 0};
    float _rotation = 0;
    float _zoom = 1.f;
    glm::vec2 _viewPort{0, 0};

    glm::mat4 _projection;
    glm::mat4 _view;

    OriginPoint _orginPoint = OriginPoint::TopLeft;

  private:
    void RecalculateViewMatrix();

  public:
    Camera(glm::vec2 viewPort);
    Camera() = default;
    void SetPosition(glm::vec2 position);
    void SetRotaion(float roation);
    void SetZoom(float zoom);
    void SetOrginPoint(OriginPoint point);
    void SetViewPort(glm::vec2 viewPort);

    const glm::vec2 GetPosition() const;
    const glm::mat4 GetProjView() const;
  };

} // namespace Base
