#include "base/shaders/ShaderManager.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/shaders/Shader.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/textures/Texture.hpp"
#include "raylib.h"
#include <iterator>
#include <memory>

namespace Base
{
  ShaderManager::ShaderManager(AssetManager *assetManager) : _assetManager(assetManager)
  {
  }

  void ShaderManager::ActivateShader(AssetHandle<Base::BaseShader> shaderHandle)
  {
    std::shared_ptr<Shader> shader = GetShader(shaderHandle);
    BeginShaderMode(*shader);
  }

  void ShaderManager::Init()
  {
    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> sig) {
      auto scenePushed = std::static_pointer_cast<ScenePushedSignal>(sig);
      UpdateCurrentScene(scenePushed->scene);
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> sig) {
      auto sceneResumed = std::static_pointer_cast<SceneResumedSignal>(sig);
      UpdateCurrentScene(sceneResumed->scene);
    });

    bus->SubscribeSignal<ScenePoppedSignal>([this](std::shared_ptr<Signal> signal) {
      auto scenePopped = std::static_pointer_cast<ScenePoppedSignal>(signal);
      ClearSceneShaderCache(scenePopped->scene);
    });
  }

  void ShaderManager::UpdateCurrentScene(const Scene *scene)
  {
    if (!_shaderCache.contains(_currentScene))
    {
      _shaderCache[scene];
    }
    _currentScene = scene;
  }

  void ShaderManager::ClearSceneShaderCache(const Scene *scene)
  {
    _shaderCache.erase(scene);
  }

  void ShaderManager::DeactivateCurrentShader()
  {
    EndShaderMode();
  }

  std::shared_ptr<Shader> ShaderManager::GetShader(AssetHandle<Base::BaseShader> shaderHandle)
  {
    if ( //
      auto it = std::ranges::find(_shaderCache.at(_currentScene), shaderHandle);
      it != _shaderCache.at(_currentScene).end() //
    )
    {
      return _shaderCache.at(_currentScene)[(int)std::distance(_shaderCache.at(_currentScene).begin(), it)]
        .Get()
        ->GetRaylibShader();
    }
    else
    {
      _shaderCache.at(_currentScene).push_back(shaderHandle);
      return shaderHandle.Get()->GetRaylibShader();
    }
  }

  void ShaderManager::SetUniform(                                                                  //
    AssetHandle<Base::BaseShader> shaderHandle, const std::string &name, const UniformValue &value //
  )
  {
    std::shared_ptr<Shader> shader = GetShader(shaderHandle);
    if (!shader)
      return;

    int loc = GetShaderLocation(*shader, name.c_str());
    if (std::holds_alternative<float>(value))
    {
      float v = std::get<float>(value);
      SetShaderValue(*shader, loc, &v, SHADER_UNIFORM_FLOAT);
    }
    else if (std::holds_alternative<Vector2>(value))
    {
      Vector2 v = std::get<Vector2>(value);
      SetShaderValueV(*shader, loc, &v, SHADER_UNIFORM_VEC2, 1);
    }
    else if (std::holds_alternative<Vector3>(value))
    {
      Vector3 v = std::get<Vector3>(value);
      SetShaderValueV(*shader, loc, &v, SHADER_UNIFORM_VEC3, 1);
    }
    else if (std::holds_alternative<Vector4>(value) || std::holds_alternative<Color>(value))
    {
      Vector4 v;
      if (std::holds_alternative<Vector4>(value))
      {
        Vector4 v = std::get<Vector4>(value);
      }
      else
      {
        Color c = std::get<Color>(value);
        v = {c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f};
      }
      SetShaderValueV(*shader, loc, &v, SHADER_UNIFORM_VEC4, 1);
    }
    else if (std::holds_alternative<int>(value))
    {
      int v = std::get<int>(value);
      SetShaderValue(*shader, loc, &v, SHADER_UNIFORM_INT);
    }
    else if (std::holds_alternative<Texture2D>(value))
    {
      Texture2D v = std::get<Texture2D>(value);
      SetShaderValueTexture(*shader, loc, v);
    }
  }

  void ShaderManager::Update(float dt)
  {
    for (AssetHandle<BaseShader> handle : _shaderCache[_currentScene])
    {
      SetUniform(handle, "u_time", (float)GetTime());
    }
  }
} // namespace Base
