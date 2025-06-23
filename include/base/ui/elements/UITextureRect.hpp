#pragma once

#include "base/assets/AssetHandle.hpp"
#include "base/textures/Texture.hpp"
#include "base/ui/UIElement.hpp"
#include "raylib.h"
namespace Base
{
  class UITextureRect : public UIElement
  {
  private:
    AssetHandle<Texture> _texturehandle;
    Rectangle _soureceRect = {0, 0, 0, 0};

  public:
    void SetTextureHandle(const AssetHandle<Texture> &handle);
    void SetSourceRect(Rectangle rect);
    void Render() override;
  };
} // namespace Base
