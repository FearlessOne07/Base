#include "base/camera/Camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <algorithm>

namespace Base
{

  Camera::Camera(glm::vec2 viewPort) : _viewPort(viewPort)
  {
    SetOrginPoint(Origin::TopLeft);
    RecalculateMatrices();
  }

  void Camera::RecalculateMatrices()
  {
    // View matrix
    glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(_position, 0.f)) *
                          glm::rotate(glm::mat4(1.f), glm::radians(_rotation), glm::vec3(0.f, 0.f, -1.f));

    _view = glm::inverse(transform);

    // Cached inverse PV
    _projView = _projection * _view;
    _invProjView = glm::inverse(_projView);
  }

  void Camera::SetPosition(glm::vec2 position)
  {
    _position = position;
    RecalculateMatrices();
  }

  void Camera::SetZoom(float zoom)
  {
    _zoom = std::clamp(zoom, 0.1f, 2.f);
    SetOrginPoint(_orginPoint);
    RecalculateMatrices();
  }

  void Camera::SetRotation(float rotation)
  {
    _rotation = rotation;
    RecalculateMatrices();
  }

  void Camera::SetViewPort(glm::vec2 viewPort)
  {
    _viewPort = viewPort;
    SetOrginPoint(_orginPoint);
    RecalculateMatrices();
  }

  void Camera::SetOrginPoint(Origin point)
  {
    _orginPoint = point;

    float left, right, bottom, top;

    switch (_orginPoint)
    {
    case Origin::Center:
      left = -(_viewPort.x * 0.5f);
      right = (_viewPort.x * 0.5f);
      top = -(_viewPort.y * 0.5f);
      bottom = (_viewPort.y * 0.5f);
      break;

    case Origin::TopLeft:
    default:
      left = 0.f;
      right = _viewPort.x;
      top = 0.f;
      bottom = _viewPort.y;
      break;
    }

    left += _offset.x;
    right += _offset.x;

    top += _offset.y;
    bottom += _offset.y;

    left /= _zoom;
    right /= _zoom;
    top /= _zoom;
    bottom /= _zoom;

    _projection = glm::ortho(left, right, bottom, top, -1.f, 1.f);
    RecalculateMatrices();
  }

  glm::vec2 Camera::GetWorldToScreen(glm::vec2 world) const
  {
    glm::vec4 clip = _projView * glm::vec4(world, 0.f, 1.f);
    glm::vec3 ndc = glm::vec3(clip) / clip.w;

    glm::vec2 screen;
    screen.x = (ndc.x + 1.f) * 0.5f * _viewPort.x;
    screen.y = (1.f - ndc.y) * 0.5f * _viewPort.y;

    return screen;
  }

  glm::vec2 Camera::GetScreenToWorld(glm::vec2 screen) const
  {
    glm::vec2 ndc;
    ndc.x = (2.f * screen.x) / _viewPort.x - 1.f;
    ndc.y = 1.f - (2.f * screen.y) / _viewPort.y;

    glm::vec4 world = _invProjView * glm::vec4(ndc, 0.f, 1.f);
    return glm::vec2(world);
  }

  float Camera::GetScreenToWorld(float distance) const
  {
    return distance / _zoom;
  }

  float Camera::GetWorldToScreen(float distance) const
  {
    return distance * _zoom;
  }

  const glm::mat4 Camera::GetProjView() const
  {
    return _projView;
  }

  const Vector2 Camera::GetPosition() const
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
