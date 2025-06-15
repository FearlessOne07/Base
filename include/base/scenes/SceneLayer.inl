#include "Scene.hpp"

namespace Base
{
  template <typename T> AssetHandle<T> SceneLayer::GetAsset(const std::string &name) const
  {
    return _owner->GetAsset<T>(name);
  }

  template <typename T> void SceneLayer::LoadAsset(const fs::path &path)
  {
    _owner->LoadAsset<T>(path, false);
  }

  template <typename T> void SceneLayer::SetSceneTransition(SceneRequest request, const SceneData &data)
  {
    _owner->SetSceneTransition<T>(request, data);
  }
} // namespace Base
