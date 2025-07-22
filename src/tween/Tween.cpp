#include "base/tween/Tween.hpp"
#include "base/util/Exception.hpp"

namespace Base
{
  template <typename T> T Tween<T>::TweenLerp(T start, T end, float t)
  {
    THROW_BASE_RUNTIME_ERROR("No Lerp function for specified type");
  };

  template <> Vector2 Tween<Vector2>::TweenLerp(Vector2 start, Vector2 end, float t)
  {
    return Vector2Lerp(start, end, t);
  };

  template <> int Tween<int>::TweenLerp(int start, int end, float t)
  {
    return Lerp(start, end, t);
  };

  template <> float Tween<float>::TweenLerp(float start, float end, float t)
  {
    return Lerp(start, end, t);
  };

  template <> unsigned char Tween<unsigned char>::TweenLerp(unsigned char start, unsigned char end, float t)
  {
    return static_cast<unsigned char>(Lerp(start, end, t));
  };

  template <> Color Tween<Color>::TweenLerp(Color start, Color end, float t)
  {
    Color color;
    color.r = static_cast<unsigned char>(Lerp(start.r, end.r, t));
    color.g = static_cast<unsigned char>(Lerp(start.g, end.g, t));
    color.b = static_cast<unsigned char>(Lerp(start.b, end.b, t));
    color.a = static_cast<unsigned char>(Lerp(start.a, end.a, t));
    return color;
  };
} // namespace Base
