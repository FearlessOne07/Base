#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/shaders/Shader.hpp"
#include "base/shaders/UniformValue.hpp"
#include "raylib.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Base
{
  class Scene;
  class AssetManager;
  class ShaderManager
  {
    friend class Game;

  private:
    Shader *_activeShader = nullptr;
    AssetManager *_assetManager = nullptr;
    const Scene *_currentScene = nullptr;
    std::shared_ptr<Shader> GetShader(AssetHandle<Base::BaseShader> shaderHandle);

    std::unordered_map<const Scene *, std::vector<AssetHandle<Base::BaseShader>>> _shaderCache;

  private:
    void Update(float dt);
    void Init();
    void UpdateCurrentScene(const Scene *scene);
    void ClearSceneShaderCache(const Scene *scene);

  public:
    ShaderManager(AssetManager *assetManager);
    void ActivateShader(AssetHandle<Base::BaseShader>);
    void DeactivateCurrentShader();
    void SetUniform(AssetHandle<Base::BaseShader> shaderName, const std::string &name, const UniformValue &value);
  };
} // namespace Base
