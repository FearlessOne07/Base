#pragma once

#include "ShaderPass.hpp"
#include "base/shaders/UniformValue.hpp"
#include <string>
#include <unordered_map>

namespace Base
{
  class ShaderChain
  {
    friend class RenderLayer;

  private:
    std::unordered_map<std::string, ShaderPass> _shaderPasses;
    // Iterator
    inline std::unordered_map<std::string, ShaderPass>::iterator begin()
    {
      return _shaderPasses.begin();
    }

    inline std::unordered_map<std::string, ShaderPass>::iterator end()
    {
      return _shaderPasses.end();
    }

  public:
    void SetShaderUniform(const std::string &shaderName, const std::string &uniformName, UniformValue value);
    void AddShaderPass(const std::string &shaderName);
    bool Empty() const;
  };
} // namespace Base
