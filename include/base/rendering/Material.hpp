#pragma once

#include "base/assets/Texture.hpp"
#include "base/shaders/Shader.hpp"
#include <memory>

namespace Base
{

  class Material
  {
  private:
    Ptr<Shader> _shader;
    std::unordered_map<std::string, UniformValue> _uniforms;
    std::unordered_map<uint8_t, std::shared_ptr<Texture>> _textures;

  public:
    Material(std::shared_ptr<Shader> shader);
    Material() = default;
    void SetUniform(const std::string &name, const UniformValue &value);
    void SetTexture(uint8_t, const std::shared_ptr<Texture> &texture);
    void Bind();
    void Unbind();
    GeometryType GetClass() const;
  };

} // namespace Base
