#include "base/scenes/SceneLayer.hpp"
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

  void SceneLayer::_onAttach(RenderLayer *renderLayer)
  {
    _renderLayer = renderLayer;
    _renderLayer->AddRenderFunction([this]() { this->Render(); });
    OnAttach();
  }

  void SceneLayer::_onDetach()
  {
    OnDetach();
  }
} // namespace Base
