#pragma once
#include "raylib.h"
#include <filesystem>
#include <unordered_map>
struct GameConfig
{
  const char *Title = "New Game";
  Vector2 MinWindowSize = {1280, 720};
  Vector2 Resolution = {1920, 1080};
  int TargetFps = 0;
  bool Vsync = false;
  bool ResizableWindow = true;
  std::unordered_map<std::string, std::filesystem::path> GlobalAssets = {};
};
