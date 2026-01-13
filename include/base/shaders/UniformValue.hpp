#pragma once
#include "base/util/Type.hpp"
#include <variant>

namespace Base
{
  using UniformValue = std::variant<float, Vector2, Vector3, Vector4, int, Color, Mat4>;
}
