#include "base/state/TransitionConditionBlock.hpp"

namespace Base
{
  TransitionEvaluationType TransitionConditionBlock::GetEvaluationType() const
  {
    return _evaluationType;
  }

  void TransitionConditionBlock::AddComponents(std::shared_ptr<Entity> e)
  {
    for (auto &comp : _components)
    {
      e->AddComponent(std::dynamic_pointer_cast<Component>(comp));
    }
  }

  void TransitionConditionBlock::RemoveComponents(std::shared_ptr<Entity> e)
  {
    for (auto &comp : _components)
    {
      e->RemoveComponent(std::dynamic_pointer_cast<Component>(comp));
    }
  }

  const std::vector<bool> &TransitionConditionBlock::GetFlags() const
  {
    return _flags;
  }

  void TransitionConditionBlock::ResetFlags()
  {
    for (size_t i = 0; i < _flags.size(); i++)
    {
      _flags[i] = false;
    }
  }

  void TransitionConditionBlock::SetIndex(size_t index)
  {
    _flags[index] = true;
  }

  const std::string &TransitionConditionBlock::GetNextState()
  {
    return _nextState;
  }
} // namespace Base
