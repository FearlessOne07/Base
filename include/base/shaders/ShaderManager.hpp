#pragma once
#include "base/assets/AssetManager.hpp"
#include "base/shaders/UniformValue.hpp"
#include "raylib.h"
#include <string>
#include <unordered_map>

namespace Base
{
  class ShaderManager
  {
  private:
    Shader *_activeShader = nullptr;
    AssetManager *_assetManager = nullptr;
    Shader *GetShader(const std::string &shaderName);

    std::unordered_map<std::string, Shader *> _shaderCache;

  public:
    ShaderManager(AssetManager *assetManager);
    void ActivateShader(const std::string &);
    void DeactivateCurrentShader();
    void SetUniform(const std::string &shaderName, const std::string &name, const UniformValue &value);
  };
} // namespace Base
