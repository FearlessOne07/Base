#pragma once
#include "base/shaders/ShaderEffect.hpp"
#include "base/util/Exception.hpp"
#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>
namespace Base
{
  class RenderLayer;
  class ShaderEffectChain
  {
  private:
    std::vector<std::shared_ptr<ShaderEffect>> _effects;
    std::vector<std::type_index> _effectIds;

  public:
    template <typename T, typename... Args>
      requires(std::is_base_of_v<ShaderEffect, T>)
    void AddEffect(std::weak_ptr<const Scene> ownerScene, Args &&...args)
    {
      auto id = std::type_index(typeid(T));

      if (std::ranges::find(_effectIds, id) == _effectIds.end())
      {
        _effectIds.push_back(id);
        std::shared_ptr<T> effect = std::make_shared<T>(std::forward<Args>(args)...);
        effect->Setup(ownerScene);
        _effects.emplace_back(std::move(effect));
      }
      else
      {
        THROW_BASE_RUNTIME_ERROR("Shader Effect already exiss in chain");
      }
    }

    template <typename T>
      requires(std::is_base_of_v<ShaderEffect, T>)
    std::shared_ptr<T> GetEffect()
    {
      auto id = std::type_index(typeid(T));

      if (auto it = std::ranges::find(_effectIds, id); it != _effectIds.end())
      {
        return std::static_pointer_cast<T>(_effects[std::distance(_effectIds.begin(), it)]);
      }
      else
      {

        THROW_BASE_RUNTIME_ERROR("Shader Effect does not exist in chain");
      }
    }

    std::vector<std::shared_ptr<ShaderEffect>>::iterator begin()
    {
      return _effects.begin();
    }

    std::vector<std::shared_ptr<ShaderEffect>>::iterator end()
    {
      return _effects.end();
    }

    std::vector<std::shared_ptr<ShaderEffect>>::const_iterator begin() const
    {
      return _effects.cbegin();
    }

    std::vector<std::shared_ptr<ShaderEffect>>::const_iterator end() const
    {
      return _effects.cend();
    }

    bool Empty() const
    {
      return _effects.empty();
    }
  };
} // namespace Base
