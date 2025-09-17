#include "base/sprites/NinePatchSprite.hpp"
#include "raymath.h"
#include <cmath>

namespace Base
{
  NinePatchSprite::NinePatchSprite( //
    const AssetHandle<Texture> &texture, Border border, Vector2 sourceIndex, Vector2 sourceSize,
    float borderScaleMultiplier //
    )
    : _texture(texture), _border(border), _sourceIndex(sourceIndex), _sourceSize(sourceSize),
      _borderScaleMultiplier(borderScaleMultiplier)
  {
    float spriteX = sourceIndex.x * sourceSize.x;
    float spriteY = sourceIndex.y * sourceSize.y;
    float spriteWidth = sourceSize.x;
    float spriteHeight = sourceSize.y;

    // Calculate the 9 source rectangles, offset by sprite position
    // Top row
    _patches[0] = {spriteX, spriteY, _border.left, border.top};                                           // Top-left
    _patches[1] = {spriteX + border.left, spriteY, spriteWidth - border.left - border.right, border.top}; // Top-center
    _patches[2] = {spriteX + spriteWidth - border.right, spriteY, border.right, border.top};              // Top-right

    // Middle row
    _patches[3] = {
      spriteX,
      spriteY + border.top,
      border.left,
      spriteHeight - border.top - border.bottom,
    }; // Middle-left

    _patches[4] = {
      spriteX + border.left,
      spriteY + border.top,
      spriteWidth - border.left - border.right,
      spriteHeight - border.top - border.bottom,
    }; // Middle-center

    _patches[5] = {
      spriteX + spriteWidth - border.right,
      spriteY + border.top,
      border.right,
      spriteHeight - border.top - border.bottom,
    }; // Middle-right

    // Bottom row
    _patches[6] = {spriteX, spriteY + spriteHeight - border.bottom, border.left, border.bottom}; // Bottom-left

    _patches[7] = {
      spriteX + border.left,
      spriteY + spriteHeight - border.bottom,
      spriteWidth - border.left - border.right,
      border.bottom,
    }; // Bottom-center

    _patches[8] = {
      spriteX + spriteWidth - border.right,
      spriteY + spriteHeight - border.bottom,
      border.right,
      border.bottom,
    }; // Bottom-right
  }
  void NinePatchSprite::SetSourceIndex(const Vector2 &index)
  {
    _sourceIndex = Vector2Max(index, {0});
  }

  void NinePatchSprite::Draw(const Rectangle &dest, unsigned char alpha)
  {
    float x = dest.x;
    float y = dest.y;
    float width = dest.width;
    float height = dest.height;

    float destArea = width * height;
    float refArea = 160 * 60;
    float _scale = _borderScaleMultiplier + 1;

    // Scale the border sizes for rendering
    float scaledBorderLeft = _border.left * _scale;
    float scaledBorderRight = _border.right * _scale;

    float scaledBorderTop = _border.top * _scale;
    float scaledBorderBottom = _border.bottom * _scale;

    // Calculate destination rectangles with scaled borders
    Rectangle destRects[9];

    // Top row
    destRects[0] = {x, y, scaledBorderLeft, scaledBorderTop};
    destRects[1] = {x + scaledBorderLeft, y, width - scaledBorderLeft - scaledBorderRight, scaledBorderTop};
    destRects[2] = {x + width - scaledBorderRight, y, scaledBorderRight, scaledBorderTop};

    // Middle row
    destRects[3] = {x, y + scaledBorderTop, scaledBorderLeft, height - scaledBorderTop - scaledBorderBottom};
    destRects[4] = {
      x + scaledBorderLeft,
      y + scaledBorderTop,
      width - scaledBorderLeft - scaledBorderRight,
      height - scaledBorderTop - scaledBorderBottom,
    };
    destRects[5] = {
      x + width - scaledBorderRight,
      y + scaledBorderTop,
      scaledBorderRight,
      height - scaledBorderTop - scaledBorderBottom,
    };

    // Bottom row
    destRects[6] = {x, y + height - scaledBorderBottom, scaledBorderLeft, scaledBorderBottom};
    destRects[7] = {
      x + scaledBorderLeft,
      y + height - scaledBorderBottom,
      width - scaledBorderLeft - scaledBorderRight,
      scaledBorderBottom,
    };
    destRects[8] = {
      x + width - scaledBorderRight,
      y + height - scaledBorderBottom,
      scaledBorderRight,
      scaledBorderBottom,
    };

    // Draw all 9 patches using the sprite sheet
    for (int i = 0; i < 9; i++)
    {
      DrawTexturePro( //
        *_texture.Get()->GetRaylibTexture(), _patches[i], destRects[i], {0, 0}, 0.0f,
        {
          static_cast<unsigned char>((255 * alpha) / 255),
          static_cast<unsigned char>((255 * alpha) / 255),
          static_cast<unsigned char>((255 * alpha) / 255),
          alpha,
        } //
      );
    }
  }
} // namespace Base
