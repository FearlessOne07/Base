#pragma once
#include "base/util/Type.hpp"
#include "internal/rendering/VertexBufferLayout.hpp"
#include <cstdint>

namespace Base
{

  struct TextureVertex
  {
    Vector3 Position;
    Vector3 Uv;
    uint32_t Mode;
    Vector4 Color;
    uint32_t ColorAttachmentMask = 0;

  private:
    static inline BufferLayout _layout;

  public:
    static void Init();
    static const BufferLayout &GetLayout();
  };

  struct CircleVertex
  {
    Vector3 Position;
    Vector4 Color;
    Vector2 Uv;
    Vector2 ThicknessFade;
    uint32_t ColorAttachmentMask = 0;

  private:
    static inline BufferLayout _layout;

  public:
    static void Init();
    static const BufferLayout &GetLayout();
  };

} // namespace Base
