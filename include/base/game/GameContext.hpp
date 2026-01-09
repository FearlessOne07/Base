#pragma once

#include "base/assets/AssetManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/renderer/Renderer.hpp"
#include "base/shaders/ShaderManager.hpp"
#include "base/systems/SystemManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"

namespace Base
{
  struct GameContext
  {
    Ref<Renderer> Rendering;
    Ref<EntityManager> Entities;
    Ref<SystemManager> Systems;
    Ref<AssetManager> Assets;
    Ref<ParticleManager> Particles;
    Ref<UIManager> Ui;
    Ref<TweenManager> Tweens;
    Ref<ShaderManager> Shaders;
  };
} // namespace Base
