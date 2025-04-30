#pragma once

#include "base/input/InputEvent.hpp"
#include <memory>
namespace Base
{
  class SceneLayer
  {
  public:
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> event) {};
  };
} // namespace Base
