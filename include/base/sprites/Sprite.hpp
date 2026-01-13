#pragma once
#include "base/assets/Texture.hpp"
#include "base/rendering/Material.hpp"
#include "base/rendering/Origin.hpp"
#include "base/rendering/Renderable.hpp"
#include "base/util/Type.hpp"
#include <memory>

namespace Base
{
  class Sprite : public Renderable
  {
  private:
    std::shared_ptr<Texture> _texture = nullptr;
    Vector2 _sourcePosition = {0, 0};
    Vector2 _sourceSize = {0, 0};
    Origin _origin;

  public:
    Sprite(std::shared_ptr<Texture> texture, Origin origin = Origin::TopLeft);
    Sprite(std::shared_ptr<Texture> texture, const Material &material, Origin origin = Origin::TopLeft);
    Sprite(std::shared_ptr<Texture> texture, Vector2 sourcePos, Vector2 sourceSize, Origin origin = Origin::TopLeft);
    Sprite( //
      std::shared_ptr<Texture> texture, Vector2 sourcePos, Vector2 sourceSize, const Material &material,
      Origin origin = Origin::TopLeft //
    );
    std::shared_ptr<Texture> GetTexture() const;
    Vector2 GetSourceSize() const;
    Vector2 GetSourcePos() const;
    Origin GetOrigin() const;
  };
} // namespace Base
