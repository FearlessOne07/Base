#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/shaders/UniformValue.hpp"
#include "raylib.h"
#include <memory>
#include <string>
#include <vector>

namespace Base
{
  class ShaderManager
  {
  private:
    Shader *_activeShader = nullptr;
    AssetManager *_assetManager = nullptr;
    std::shared_ptr<Shader> GetShader(AssetHandle<Shader> shaderHandle);

    std::vector<AssetHandle<Shader>> _shaderCache;

  public:
    ShaderManager(AssetManager *assetManager);
    void ActivateShader(AssetHandle<Shader>);
    void DeactivateCurrentShader();
    void SetUniform(AssetHandle<Shader> shaderName, const std::string &name, const UniformValue &value);
  };
} // namespace Base
