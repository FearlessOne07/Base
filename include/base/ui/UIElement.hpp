#pragma once

#include "base/assets/AssetHandle.hpp"
#include "base/input/InputEvent.hpp"
#include "base/textures/Font.hpp"
#include <functional>
#include <memory>
#include <raylib.h>

namespace Base
{
  class UIElement
  {
    friend class UILayer;

  public:
    enum class AnchorPoint : uint8_t
    {
      CENTER = 0,
      TOP_LEFT,
      TOP_RIGHT,
      BOTTOM_LEFT,
      BOTTOM_RIGHT,
    };

  protected:
    AssetHandle<BaseFont> _font;
    Vector2 _position = {0, 0};
    Vector2 _setPosition = {0, 0};

    Vector2 _size = {0, 0};
    AnchorPoint _anchorPoint = AnchorPoint::TOP_LEFT;

  private:
    void UpdatePosition();

    // Template Methods
    void _update(float dt);

  public:
    void SetAnchorPoint(AnchorPoint anchorPoint);
    void SetPosition(Vector2 position);
    void SetFont(const AssetHandle<BaseFont> &);
    Vector2 GetPosition() const;
    Vector2 GetSize() const;

    virtual void Update(float dt);
    virtual void Render() = 0;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event);

    std::function<void()> OnShow = nullptr;
    std::function<void()> OnHide = nullptr;
  };
} // namespace Base
