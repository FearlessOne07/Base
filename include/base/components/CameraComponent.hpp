#pragma once
#include "base/Component.hpp"
#include <raylib.h>

namespace Base
{
  enum struct CameraMode : uint8_t
  {
    BASIC_FOLLOW = 0,
    SMOOTH_FOLLOW = 1
  };

  struct CameraComponent : public Component
  {
    Vector2 target = {0, 0};
    CameraMode cameraMode;
    float maxFollowDistance = 0.f;
    float cameraSpeed = 0.f;
  };
} // namespace Base
