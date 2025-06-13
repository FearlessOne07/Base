#pragma once

#include "ShaderPass.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/shaders/Shader.hpp"
#include "base/shaders/UniformValue.hpp"
#include <string>
#include <unordered_map>

namespace Base
{
  class ShaderChain
  {
    friend class RenderLayer;

  private:
    std::unordered_map<AssetHandle<Base::BaseShader>, ShaderPass> _shaderPasses;
    // Iterator
    inline std::unordered_map<AssetHandle<Base::BaseShader>, ShaderPass>::iterator begin()
    {
      return _shaderPasses.begin();
    }

    inline std::unordered_map<AssetHandle<Base::BaseShader>, ShaderPass>::iterator end()
    {
      return _shaderPasses.end();
    }

  public:
    void SetShaderUniform(                                                                           //
      AssetHandle<Base::BaseShader> shaderHandle, const std::string &uniformName, UniformValue value //
    );
    void AddShaderPass(AssetHandle<Base::BaseShader> shaderHandle);
    bool Empty() const;
  };
} // namespace Base
