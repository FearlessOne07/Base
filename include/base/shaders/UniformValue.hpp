#pragma once
#include "raylib.h"
#include <variant>
using UniformValue = std::variant<float, Vector2, Vector3, Vector4, int, Color>;
