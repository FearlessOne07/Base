#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/shaders/Shader.hpp"
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
    std::shared_ptr<Shader> GetShader(AssetHandle<Base::BaseShader> shaderHandle);

    std::vector<AssetHandle<Base::BaseShader>> _shaderCache;

  public:
    ShaderManager(AssetManager *assetManager);
    void ActivateShader(AssetHandle<Base::BaseShader>);
    void DeactivateCurrentShader();
    void SetUniform(AssetHandle<Base::BaseShader> shaderName, const std::string &name, const UniformValue &value);
  };
} // namespace Base
