#pragma once
#include "base/shaders/ShaderEffect.hpp"
#include "base/util/Exception.hpp"
#include <algorithm>
#include <iterator>
#include <memory>
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
    template <typename T, typename... Args> void AddEffect(const Scene *currentScene, Args &&...args)
    {
      if (std::is_base_of_v<ShaderEffect, T>)
      {
        auto id = std::type_index(typeid(T));

        if (std::ranges::find(_effectIds, id) == _effectIds.end())
        {
          _effectIds.push_back(id);
          std::shared_ptr<T> effect = std::make_shared<T>(std::forward<Args>(args)...);
          effect->Setup(currentScene);
          _effects.emplace_back(std::move(effect));
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Shader Effect already exiss in chain");
        }
      }
      else
      {
        THROW_BASE_RUNTIME_ERROR("T must be a base of ShaderEffect");
      }
    }

    template <typename T> std::shared_ptr<T> GetEffect()
    {
      if (std::is_base_of_v<ShaderEffect, T>)
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
      else
      {
        THROW_BASE_RUNTIME_ERROR("T must be a base of ShaderEffect");
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
