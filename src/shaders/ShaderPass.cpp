#include "base/shaders/ShaderPass.hpp"
#include "base/util/Exception.hpp"
#include <algorithm>

namespace Base
{

  UniformValue ShaderPass::GetUniformValue(const std::string &uniform)
  {
    if (_uniformValues.contains(uniform))
    {
      return _uniformValues.at(uniform);
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Uniform " + uniform + " hasn't been set");
    }
  }

  void ShaderPass::SetUniformValue(const std::string &uniformName, UniformValue value)
  {
    _uniformValues[uniformName] = value;

    if (std::ranges::find(_dirty, uniformName) == _dirty.end())
    {
      _dirty.push_back(uniformName);
    }
    _hasDirty = true;
  }

  const std::vector<std::string> &ShaderPass::GetDirty() const
  {
    return _dirty;
  }

  bool ShaderPass::HasDirty() const
  {
    return _hasDirty;
  }

  void ShaderPass::ClearDirty()
  {
    _dirty.clear();
    _hasDirty = false;
  }

  ShaderPass::ShaderPass()
  {
  }
} // namespace Base
