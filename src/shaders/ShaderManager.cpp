#include "base/shaders/ShaderManager.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/shaders/Shader.hpp"
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

  void ShaderManager::DeactivateCurrentShader()
  {
    EndShaderMode();
  }

  std::shared_ptr<Shader> ShaderManager::GetShader(AssetHandle<Base::BaseShader> shaderHandle)
  {
    if (auto it = std::ranges::find(_shaderCache, shaderHandle); it != _shaderCache.end())
    {
      return _shaderCache[(int)std::distance(_shaderCache.begin(), it)].Get()->GetRaylibShader();
    }
    else
    {
      _shaderCache.push_back(shaderHandle);
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
  }
} // namespace Base
