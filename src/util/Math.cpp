#include "base/util/Math.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/trigonometric.hpp"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

namespace Base
{

  /* ============================= */
  /* Scalars (float)               */
  /* ============================= */

  float Math::Clamp(float v, float min, float max)
  {
    return std::clamp(v, min, max);
  }

  float Math::Lerp(float a, float b, float t)
  {
    return a + (b - a) * t;
  }

  float Math::Abs(float v)
  {
    return std::abs(v);
  }

  float Math::Min(float a, float b)
  {
    return std::min(a, b);
  }

  float Math::Max(float a, float b)
  {
    return std::max(a, b);
  }

  /* ============================= */
  /* Scalars (int)                 */
  /* ============================= */

  int32_t Math::Clamp(int32_t v, int32_t min, int32_t max)
  {
    return std::clamp(v, min, max);
  }

  int32_t Math::Abs(int32_t v)
  {
    return std::abs(v);
  }

  int32_t Math::Min(int32_t a, int32_t b)
  {
    return std::min(a, b);
  }

  int32_t Math::Max(int32_t a, int32_t b)
  {
    return std::max(a, b);
  }

  int32_t Math::GCD(int32_t a, int32_t b)
  {
    a = std::abs(a);
    b = std::abs(b);

    while (b != 0)
    {
      int32_t t = b;
      b = a % b;
      a = t;
    }
    return a;
  }

  int32_t Math::LCM(int32_t a, int32_t b)
  {
    if (a == 0 || b == 0)
      return 0;

    return std::abs(a / GCD(a, b) * b);
  }

  /* ============================= */
  /* Vector2                       */
  /* ============================= */

  Vector2 Math::Clamp(const Vector2 &v, float min, float max)
  {
    return glm::clamp(v, min, max);
  }

  Vector2 Math::Lerp(const Vector2 &a, const Vector2 &b, float t)
  {
    return glm::mix(a, b, t);
  }

  float Math::Dot(const Vector2 &a, const Vector2 &b)
  {
    return glm::dot(a, b);
  }

  float Math::Length(const Vector2 &v)
  {
    return glm::length(v);
  }

  Vector2 Math::Normalize(const Vector2 &v)
  {
    return glm::normalize(v);
  }

  Vector2 Math::Abs(const Vector2 &v)
  {
    return glm::abs(v);
  }

  Vector2 Math::Min(const Vector2 &a, const Vector2 &b)
  {
    return glm::min(a, b);
  }

  Vector2 Math::Max(const Vector2 &a, const Vector2 &b)
  {
    return glm::max(a, b);
  }

  /* ============================= */
  /* Vector3                       */
  /* ============================= */

  Vector3 Math::Clamp(const Vector3 &v, float min, float max)
  {
    return glm::clamp(v, min, max);
  }

  Vector3 Math::Lerp(const Vector3 &a, const Vector3 &b, float t)
  {
    return glm::mix(a, b, t);
  }

  float Math::Dot(const Vector3 &a, const Vector3 &b)
  {
    return glm::dot(a, b);
  }

  float Math::Length(const Vector3 &v)
  {
    return glm::length(v);
  }

  Vector3 Math::Normalize(const Vector3 &v)
  {
    return glm::normalize(v);
  }

  Vector3 Math::Abs(const Vector3 &v)
  {
    return glm::abs(v);
  }

  Vector3 Math::Min(const Vector3 &a, const Vector3 &b)
  {
    return glm::min(a, b);
  }

  Vector3 Math::Max(const Vector3 &a, const Vector3 &b)
  {
    return glm::max(a, b);
  }

  /* ============================= */
  /* Vector4                       */
  /* ============================= */

  Vector4 Math::Clamp(const Vector4 &v, float min, float max)
  {
    return glm::clamp(v, min, max);
  }

  Vector4 Math::Lerp(const Vector4 &a, const Vector4 &b, float t)
  {
    return glm::mix(a, b, t);
  }

  float Math::Dot(const Vector4 &a, const Vector4 &b)
  {
    return glm::dot(a, b);
  }

  float Math::Length(const Vector4 &v)
  {
    return glm::length(v);
  }

  Vector4 Math::Normalize(const Vector4 &v)
  {
    return glm::normalize(v);
  }

  Vector4 Math::Abs(const Vector4 &v)
  {
    return glm::abs(v);
  }

  Vector4 Math::Min(const Vector4 &a, const Vector4 &b)
  {
    return glm::min(a, b);
  }

  Vector4 Math::Max(const Vector4 &a, const Vector4 &b)
  {
    return glm::max(a, b);
  }

  float Math::ToDegrees(float radians)
  {
    return glm::degrees(radians);
  }

  float Math::ToRadians(float degrees)
  {
    return glm::radians(degrees);
  }

  float Math::Pi()
  {
    return glm::pi<float>();
  }
} // namespace Base
