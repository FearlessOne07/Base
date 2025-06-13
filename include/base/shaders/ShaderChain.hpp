#pragma once

#include "ShaderPass.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/shaders/UniformValue.hpp"
#include <string>
#include <unordered_map>

namespace Base
{
  class ShaderChain
  {
    friend class RenderLayer;

  private:
    std::unordered_map<AssetHandle<Shader>, ShaderPass> _shaderPasses;
    // Iterator
    inline std::unordered_map<AssetHandle<Shader>, ShaderPass>::iterator begin()
    {
      return _shaderPasses.begin();
    }

    inline std::unordered_map<AssetHandle<Shader>, ShaderPass>::iterator end()
    {
      return _shaderPasses.end();
    }

  public:
    void SetShaderUniform(AssetHandle<Shader> shaderHandle, const std::string &uniformName, UniformValue value);
    void AddShaderPass(AssetHandle<Shader> shaderHandle);
    bool Empty() const;
  };
} // namespace Base
