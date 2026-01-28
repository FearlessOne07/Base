#pragma once
#include "base/components/TransitionComponent.hpp"
#include "base/entities/Entity.hpp"
#include "base/util/Exception.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Base
{
  enum class TransitionEvaluationType
  {
    Or = 0,
    And
  };
  class TransitionConditionBlock
  {
    std::vector<std::shared_ptr<TransitionComponent>> _components;
    std::vector<bool> _flags;
    TransitionEvaluationType _evaluationType = TransitionEvaluationType::Or;
    std::string _nextState = "";

  public:
    template <typename... Components>
    TransitionConditionBlock(std::string nextState, TransitionEvaluationType type, Components &&...components)
      : _evaluationType(type), _nextState(nextState)
    {
      auto process = [this](auto &&c) {
        using T = std::decay_t<decltype(c)>;

        // Determine the type we're actually pointing to
        using Pointee = std::conditional_t<std::is_pointer_v<T>, std::remove_pointer_t<T>, typename T::element_type>;

        if constexpr (!std::is_base_of_v<TransitionComponent, Pointee>)
        {
          THROW_BASE_RUNTIME_ERROR("Component does not inherit from TransitionComponent");
        }
        _components.push_back(std::forward<decltype(c)>(c));
        _flags.push_back(false);
      };
      (process(std::forward<Components>(components)), ...);

      size_t currentIndex = 0;
      for (auto &c : _components)
      {
        c->SetBlockIndex(currentIndex++);
      }
    }

    TransitionConditionBlock() = default;

    TransitionEvaluationType GetEvaluationType() const;

    void AddComponents(std::shared_ptr<Entity> e);

    void RemoveComponents(std::shared_ptr<Entity> e);

    const std::vector<bool> &GetFlags() const;
    void ResetFlags();
    void SetIndex(size_t index);
    const std::string &GetNextState();
  };
} // namespace Base
