#pragma once
#include "base/assets/AssetManager.hpp"

#include "base/entities/EntityManager.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/rendering/RenderingManager.hpp"
#include "base/systems/SystemManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"

namespace Base
{
  struct EngineCtx
  {
    Ref<RenderingManager> Rendering;
    Ref<EntityManager> Entities;
    Ref<ParticleManager> Particles;
    Ref<AssetManager> Assets;
    Ref<SystemManager> Systems;
    Ref<UIManager> Ui;
    Ref<TweenManager> Tweens;
  };
} // namespace Base
