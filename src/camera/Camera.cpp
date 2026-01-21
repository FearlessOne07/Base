#include "base/camera/Camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include <algorithm>

namespace Base
{

Camera::Camera(glm::vec2 viewPort) : _viewPort(viewPort)
{
  // Init Matrices
  SetOrginPoint(OriginPoint::TopLeft);
  RecalculateViewMatrix();
}

void Camera::RecalculateViewMatrix()
{
  glm::mat4 transform = //
    glm::translate(glm::mat4(1.f), glm::vec3(_position.x, _position.y, 0.f)) *
    glm::rotate(glm::mat4(1.f), glm::radians(_rotation), glm::vec3(0.f, 0.f, -1.f));
  _view = glm::inverse(transform);
}

void Camera::SetPosition(glm::vec2 position)
{
  _position = position;
  RecalculateViewMatrix();
}

void Camera::SetZoom(float zoom)
{
  _zoom = zoom;
  _zoom = std::clamp<float>(_zoom, 0.1f, 2.f);
  SetOrginPoint(_orginPoint);
  RecalculateViewMatrix();
}

void Camera::SetRotation(float rotation)
{
  _rotation = rotation;
  RecalculateViewMatrix();
}

void Camera::SetViewPort(glm::vec2 viewPort)
{
  _viewPort = viewPort;
  SetOrginPoint(_orginPoint);
}

void Camera::SetOrginPoint(OriginPoint point)
{
  _orginPoint = point;
  float left, right, bottom, top;

  switch (_orginPoint)
  {
  case OriginPoint::Center:
    left = -(_viewPort.x / 2.f);
    right = _viewPort.x / 2.f;
    top = -(_viewPort.y / 2.f);
    bottom = _viewPort.y / 2.f;
    break;
  case OriginPoint::TopLeft:
    left = 0;
    right = _viewPort.x;
    top = 0;
    bottom = _viewPort.y;
    break;
  }
  left /= _zoom;
  right /= _zoom;
  bottom /= _zoom;
  top /= _zoom;
  _projection = glm::ortho(left, right, bottom, top, -1.f, 1.f);
}

const glm::mat4 Camera::GetProjView() const
{
  return _projection * _view;
}

const glm::vec2 Camera::GetPosition() const
{
  return _position;
}

const float Camera::GetZoom() const
{
  return _zoom;
}

const float Camera::GetRotation() const
{
  return _rotation;
}
} // namespace Base
