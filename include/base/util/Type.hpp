#pragma once
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <memory>

namespace Base
{
  using Vector2 = glm::vec2;
  using Vector3 = glm::vec3;
  using Vector4 = glm::vec4;
  using Color = glm::u8vec4;

  template <typename T> using Ptr = std::shared_ptr<T>;
} // namespace Base
