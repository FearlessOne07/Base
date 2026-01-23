#pragma once

#include "Type.hpp" // wherever your Vector2/3/4 typedefs live
#include <cstdint>

namespace Base
{
  class Math
  {
  public:
    /* ============================= */
    /* Scalars (float)               */
    /* ============================= */

    static float Clamp(float v, float min, float max);
    static float Lerp(float a, float b, float t);
    static float Abs(float v);
    static float Min(float a, float b);
    static float Max(float a, float b);

    /* ============================= */
    /* Scalars (int)                 */
    /* ============================= */

    static int32_t Clamp(int32_t v, int32_t min, int32_t max);
    static int32_t Abs(int32_t v);
    static int32_t Min(int32_t a, int32_t b);
    static int32_t Max(int32_t a, int32_t b);
    static int32_t GCD(int32_t a, int32_t b);
    static int32_t LCM(int32_t a, int32_t b);

    /* ============================= */
    /* Vector2                       */
    /* ============================= */

    static Vector2 Clamp(const Vector2 &v, float min, float max);
    static Vector2 Lerp(const Vector2 &a, const Vector2 &b, float t);
    static float Dot(const Vector2 &a, const Vector2 &b);
    static float Length(const Vector2 &v);
    static Vector2 Normalize(const Vector2 &v);
    static Vector2 Abs(const Vector2 &v);
    static Vector2 Min(const Vector2 &a, const Vector2 &b);
    static Vector2 Max(const Vector2 &a, const Vector2 &b);

    /* ============================= */
    /* Vector3                       */
    /* ============================= */

    static Vector3 Clamp(const Vector3 &v, float min, float max);
    static Vector3 Lerp(const Vector3 &a, const Vector3 &b, float t);
    static float Dot(const Vector3 &a, const Vector3 &b);
    static float Length(const Vector3 &v);
    static Vector3 Normalize(const Vector3 &v);
    static Vector3 Abs(const Vector3 &v);
    static Vector3 Min(const Vector3 &a, const Vector3 &b);
    static Vector3 Max(const Vector3 &a, const Vector3 &b);

    /* ============================= */
    /* Vector4                       */
    /* ============================= */

    static Vector4 Clamp(const Vector4 &v, float min, float max);
    static Vector4 Lerp(const Vector4 &a, const Vector4 &b, float t);
    static float Dot(const Vector4 &a, const Vector4 &b);
    static float Length(const Vector4 &v);
    static Vector4 Normalize(const Vector4 &v);
    static Vector4 Abs(const Vector4 &v);
    static Vector4 Min(const Vector4 &a, const Vector4 &b);
    static Vector4 Max(const Vector4 &a, const Vector4 &b);

    /* ============================= */
    /* Angles                        */
    /* ============================= */

    static float ToDegrees(float radians);
    static float ToRadians(float degrees);
    static float Pi();
  };
} // namespace Base
