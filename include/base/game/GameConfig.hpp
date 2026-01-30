#pragma once
#include "base/util/Type.hpp"

namespace Base
{
  struct GameConfig
  {
    const char *Title = "New Game";
    IVector2 MinWindowSize = {1280, 720};
    IVector2 Resolution = {1920, 1080};
    int TargetFps = 0;
    bool Vsync = false;
    bool ResizableWindow = true;
  };
} // namespace Base
