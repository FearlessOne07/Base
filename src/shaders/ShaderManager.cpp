#include "base/shaders/ShaderManager.hpp"
#include "raylib.h"

namespace Base
{

  ShaderManager::ShaderManager(AssetManager *assetManager) : _assetManager(assetManager)
  {
  }

  void ShaderManager::ActivateShader(const std::string &shaderName)
  {
    Shader *shader = GetShader(shaderName);
    BeginShaderMode(*shader);
  }

  void ShaderManager::DeactivateCurrentShader()
  {
    EndShaderMode();
  }

  Shader *ShaderManager::GetShader(const std::string &shaderName)
  {
    if (_shaderCache.contains(shaderName))
    {
      return _shaderCache.at(shaderName);
    }
    else
    {
      Shader *shader = _assetManager->GetAsset<Shader>(shaderName).get();
      _shaderCache.insert({shaderName, shader});
      return shader;
    }
  }

  void ShaderManager::SetUniform(const std::string &shaderName, const std::string &name, const UniformValue &value)
  {
    Shader *shader = GetShader(shaderName);
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
