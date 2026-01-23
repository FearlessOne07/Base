#pragma once
#include "base/assets/Texture.hpp"
#include "base/rendering/Origin.hpp"
#include "base/rendering/Renderable.hpp"
#include "glm/fwd.hpp"
#include <memory>

namespace Base
{
  class Sprite : public Renderable
  {
  private:
    Ptr<Texture> _texture = nullptr;
    glm::vec2 _sourcePosition = {0, 0};
    glm::vec2 _sourceSize = {0, 0};
    Origin _origin;

  public:
    Sprite() = default;
    Sprite(std::shared_ptr<Texture> texture, Origin origin = Origin::TopLeft);
    Sprite(std::shared_ptr<Texture> texture, const Material &material, Origin origin = Origin::TopLeft);
    Sprite(std::shared_ptr<Texture> texture, glm::vec2 sourcePos, glm::vec2 sourceSize,
           Origin origin = Origin::TopLeft);
    Sprite( //
      Ptr<Texture> texture, glm::vec2 sourcePos, glm::vec2 sourceSize, const Material &material,
      Origin origin = Origin::TopLeft //
    );
    std::shared_ptr<Texture> GetTexture() const;
    Vector2 GetSourceSize() const;
    Vector2 GetSourcePos() const;
    Origin GetOrigin() const;

    void SetSourcePos(Vector2 pos);
    void SetSourceSize(Vector2 size);

    operator bool();
  };
} // namespace Base
