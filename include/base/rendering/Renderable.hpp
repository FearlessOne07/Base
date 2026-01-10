#pragma once

#include "base/rendering/Material.hpp"

namespace Base
{

  class Renderable
  {
  private:
    Material _material;
    bool _hasMaterial = false;

  public:
    Renderable()
    {
    }

    Renderable(const Material &material) : _material(material)
    {
      _hasMaterial = true;
    }

    void SetMaterial(const Material &material)
    {
      _material = material;
      _hasMaterial = true;
    }

    Material GetMaterial() const
    {
      return _material;
    }

    bool HasMaterial() const
    {
      return _hasMaterial;
    }

    void ClearMaterial()
    {
      _hasMaterial = false;
    }
  };

} // namespace Base
