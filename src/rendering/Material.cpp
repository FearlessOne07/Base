#include "base/rendering/Material.hpp"
#include <iostream>

namespace Base
{
  void Material::Bind()
  {
    for (auto &[uniform, value] : _uniforms)
    {
      _shader->SetUniform(uniform, value);
    }

    _shader->Use();
    for (auto &[slot, texture] : _textures)
    {
      texture->Bind(slot);
    }
  }

  Material::Material(std::shared_ptr<Shader> shader) : _shader(shader)
  {
  }

  GeometryType Material::GetClass() const
  {
    return _shader->GetClass();
  }

  void Material::Unbind()
  {
    _shader->Unuse();
  }

  void Material::SetUniform(const std::string &name, const UniformValue &value)
  {
    _uniforms.insert_or_assign(name, value);
  }

  void Material::SetTexture(uint8_t slot, const std::shared_ptr<Texture> &texture)
  {
    if (slot < 1)
    {
      std::cout << "Warning: Slot 0 is reserved. Texture not bound in material\n";
    }
    else
    {
      _textures[slot] = texture;
    }
  }

} // namespace Base
