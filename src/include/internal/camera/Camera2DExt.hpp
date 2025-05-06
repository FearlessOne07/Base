#pragma once

#include "raylib.h"
#include <cstdint>
namespace Base
{
  class Camera2DExt
  {
  public:
    // Base Raylib Camera
    Camera2D camera = {
      .offset = {0, 0},
      .target = {0, 0},
      .rotation = 0,
      .zoom = 1,
    };

    // Spec
    Vector2 target = {0, 0};
    CameraMode cameraMode;
    float maxFollowDistance = 0.f;
    float cameraSpeed = 0.f;
  };
} // namespace Base
