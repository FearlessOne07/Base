#pragma once
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <memory>

namespace Base
{
  using Vector2 = glm::vec2;
  using Vector3 = glm::vec3;
  using Vector4 = glm::vec4;
  using IVector2 = glm::ivec2;
  using IVector3 = glm::ivec3;
  using IVector4 = glm::ivec4;
  using Mat4 = glm::mat4;
  using Color = glm::u8vec4;

  template <typename T> using Ptr = std::shared_ptr<T>;
} // namespace Base
