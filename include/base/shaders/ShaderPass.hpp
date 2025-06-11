#pragma once

#include "base/shaders/UniformValue.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace Base
{
  class ShaderPass
  {
    std::unordered_map<std::string, UniformValue> _uniformValues;
    std::vector<std::string> _dirty;
    bool _hasDirty = false;

  public:
    ShaderPass();

    // Uniforms
    UniformValue GetUniformValue(const std::string &uniform);
    void SetUniformValue(const std::string &uniformName, UniformValue value);

    // Dirty
    const std::vector<std::string> &GetDirty() const;
    bool HasDirty() const;
    void ClearDirty();
  };
} // namespace Base
