#pragma once
#include "Component.hpp"
#include <raylib.h>

namespace Base
{
  enum struct CameraMode : uint8_t
  {
    STATIC = 0,
    BASIC_FOLLOW,
    SMOOTH_FOLLOW
  };

  struct CameraComponent : public Component
  {
    Vector2 target = {0, 0};
    CameraMode cameraMode;
    float maxFollowDistance = 0.f;
    float cameraSpeed = 0.f;
  };
} // namespace Base
