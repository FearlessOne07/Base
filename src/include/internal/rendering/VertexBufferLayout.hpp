#pragma once
#include "glad/glad.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace Base
{
  enum class ShaderDataType
  {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool,
    Uint,
    Uint2,
    Uint3,
    Uint4,
  };

  static int32_t GetShaderDataTypeSize(ShaderDataType type)
  {
    switch (type)
    {
    case ShaderDataType::None:
      return 0;
    case ShaderDataType::Float:
      return 4;
    case ShaderDataType::Float2:
      return 4 * 2;
    case ShaderDataType::Float3:
      return 4 * 3;
    case ShaderDataType::Float4:
      return 4 * 4;
    case ShaderDataType::Mat3:
      return 4 * 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4 * 4;
    case ShaderDataType::Int:
      return 4;
    case ShaderDataType::Int2:
      return 4 * 2;
    case ShaderDataType::Int3:
      return 4 * 3;
    case ShaderDataType::Int4:
      return 4 * 4;
    case ShaderDataType::Bool:
      return 1;
    case ShaderDataType::Uint:
      return 4;
    case ShaderDataType::Uint2:
      return 4 * 2;
    case ShaderDataType::Uint3:
      return 4 * 3;
    case ShaderDataType::Uint4:
      return 4 * 4;
    }
    std::cerr << "Invlaid Shdaer Type";
    exit(-1);
  }

  inline GLenum ShaderTypeToGLType(ShaderDataType type)
  {
    switch (type)
    {

    case ShaderDataType::None:
      return 0;
    case ShaderDataType::Float:
      return GL_FLOAT;
    case ShaderDataType::Float2:
      return GL_FLOAT;
    case ShaderDataType::Float3:
      return GL_FLOAT;
    case ShaderDataType::Float4:
      return GL_FLOAT;
    case ShaderDataType::Mat3:
      return GL_FLOAT;
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
      return GL_INT;
    case ShaderDataType::Int2:
      return GL_INT;
    case ShaderDataType::Int3:
      return GL_INT;
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Uint:
      return GL_UNSIGNED_INT;
    case ShaderDataType::Uint2:
      return GL_UNSIGNED_INT;
    case ShaderDataType::Uint3:
      return GL_UNSIGNED_INT;
    case ShaderDataType::Uint4:
      return GL_UNSIGNED_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
      break;
    }
    std::cerr << "Unkown Shader Type\n";
    exit(-1);
  }
  struct BufferElement
  {
    ShaderDataType Type = ShaderDataType::None;
    size_t Offset = 0;
    size_t Size = 0;
    bool Normalized = false;

    BufferElement(ShaderDataType type, bool normalized = false)
      : Type(type), Size(GetShaderDataTypeSize(type)), Normalized(normalized)
    {
    }

    int32_t GetComponentCount() const
    {
      switch (Type)
      {
      case ShaderDataType::Float:
        return 1;
      case ShaderDataType::Float2:
        return 2;
      case ShaderDataType::Float3:
        return 3;
      case ShaderDataType::Float4:
        return 4;
      case ShaderDataType::Int:
        return 1;
      case ShaderDataType::Int2:
        return 2;
      case ShaderDataType::Int3:
        return 3;
      case ShaderDataType::Int4:
        return 4;
      case ShaderDataType::Bool:
        return 1;
      case ShaderDataType::Mat3:
        return 9; // 3x3
      case ShaderDataType::Mat4:
        return 16; // 4x4
      default:
        return 0;
      }
      exit(-1);
    }
  };

  class BufferLayout
  {
    std::vector<BufferElement> _elements;
    uint32_t _stride = 0;

  private:
    void CalculateOffsetsAndStride()
    {

      uint32_t offset = 0;
      for (auto &element : _elements)
      {
        element.Offset = offset;
        _stride += element.Size;
        offset += element.Size;
      }
    }

  public:
    std::vector<BufferElement>::const_iterator begin() const
    {
      return _elements.begin();
    }

    std::vector<BufferElement>::const_iterator end() const
    {
      return _elements.end();
    }

    uint32_t GetStride() const
    {
      return _stride;
    }
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement> &list) : _elements(list)
    {
      CalculateOffsetsAndStride();
    }
  };

} // namespace Base
