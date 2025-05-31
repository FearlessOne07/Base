#include "base/scenes/SceneLayer.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"

namespace Base
{
  Scene *SceneLayer::GetOwner()
  {
    return _owner;
  }

  RenderLayer *SceneLayer::GetRenderLayer()
  {
    return _renderLayer;
  }

  void SceneLayer::_onAttach()
  {
    auto rd = Base::RenderContextSingleton::GetInstance();
    _renderLayer = GetOwner()->GetRenderer()->InitLayer(                                //
      GetOwner(), {0, 0}, {rd->gameWidth, rd->gameHeight}, [this]() { this->Render(); } //
    );

    OnAttach();
  }

  void SceneLayer::_onDetach()
  {
    OnDetach();
  }
} // namespace Base
