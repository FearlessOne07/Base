#include "base/shaders/ShaderChain.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/util/Exception.hpp"

namespace Base
{

  void ShaderChain::SetShaderUniform( //
    AssetHandle<Shader> shaderHandle, const std::string &uniformName,
    UniformValue value //
  )
  {
    if (_shaderPasses.contains(shaderHandle))
    {
      _shaderPasses.at(shaderHandle).SetUniformValue(uniformName, value);
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Shader not defined in shaderchain");
    }
  }

  void ShaderChain::AddShaderPass(AssetHandle<Shader> shaderHandle)
  {
    if (!_shaderPasses.contains(shaderHandle))
    {
      _shaderPasses.insert({shaderHandle, ShaderPass()});
    }
  }

  bool ShaderChain::Empty() const
  {
    return _shaderPasses.empty();
  }

} // namespace Base
