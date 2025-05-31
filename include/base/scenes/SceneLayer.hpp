#pragma once

#include "base/input/InputEvent.hpp"
#include "base/renderer/RenderLayer.hpp"
#include <memory>
namespace Base
{
  class Scene;
  class SceneLayer
  {
    friend class SceneLayerStack;
    Scene *_owner = nullptr;
    RenderLayer *_renderLayer = nullptr;

  private:
    void _onAttach();
    void _onDetach();

  protected:
    Scene *GetOwner();
    RenderLayer *GetRenderLayer();

  public:
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event) {};
  };
} // namespace Base
