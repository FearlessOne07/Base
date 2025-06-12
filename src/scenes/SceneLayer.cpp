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

  void SceneLayer::_onAttach()
  {
    OnAttach();
  }

  void SceneLayer::_onDetach()
  {
    OnDetach();
  }
  
// TODO: Make Attach do this
  void SceneLayer::SetRenderLayer(RenderLayer *layer)
  {
    _renderLayer = layer;
    layer->AddRenderFunction([this]() { this->Render(); });
  }
} // namespace Base
