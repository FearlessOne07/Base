#include "base/shaders/ShaderChain.hpp"
#include "base/util/Exception.hpp"

namespace Base
{

  void ShaderChain::SetShaderUniform(const std::string &shaderName, const std::string &uniformName, UniformValue value)
  {
    if (_shaderPasses.contains(shaderName))
    {
      _shaderPasses.at(shaderName).SetUniformValue(uniformName, value);
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Shader " + shaderName + " not defined in shaderchain");
    }
  }

  void ShaderChain::AddShaderPass(const std::string &shaderName)
  {
    if (!_shaderPasses.contains(shaderName))
    {
      _shaderPasses.insert({shaderName, ShaderPass()});
    }
  }

  bool ShaderChain::Empty() const
  {
    return _shaderPasses.empty();
  }

} // namespace Base
