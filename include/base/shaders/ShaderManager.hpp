#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/shaders/Shader.hpp"
#include "base/shaders/UniformValue.hpp"
#include "base/util/Ref.hpp"
#include "raylib.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Base
{
  class SceneID;
  class AssetManager;
  class ShaderManager
  {
    friend class Game;

  private:
    Shader *_activeShader = nullptr;
    Ref<AssetManager> _assetManager;
    SceneID _currentScene;
    std::shared_ptr<Shader> GetShader(AssetHandle<Base::BaseShader> shaderHandle);

    std::unordered_map<SceneID, std::vector<AssetHandle<Base::BaseShader>>> _shaderCache;

  private:
    void Update(float dt);
    void Init();
    void UpdateCurrentScene(SceneID);
    void ClearSceneShaderCache(SceneID);

  public:
    ShaderManager(Ref<AssetManager> assetManager);
    void ActivateShader(AssetHandle<Base::BaseShader>);
    void DeactivateCurrentShader();
    void SetUniform(AssetHandle<Base::BaseShader> shaderName, const std::string &name, const UniformValue &value);
  };
} // namespace Base
