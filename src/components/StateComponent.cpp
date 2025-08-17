#include "base/components/StateComponent.hpp"
#include <string>

namespace Base
{

  StateComponent::StateComponent(                                                         //
    const std::string &initialState, const std::unordered_map<std::string, State> &states //
    )
    : _initialState(initialState), _states(states)
  {
  }

  void StateComponent::SwitchState(const std::string &newState, bool initialize)
  {
    if (_currentState != "")
    {
      _states.at(_currentState).compBundle.RemoveComponents(GetOwner());
      _states.at(_currentState).transitionBlock.RemoveComponents(GetOwner());
    }

    if (initialize)
    {
      _currentState = _initialState;
      _stateInitialized = true;
    }
    else
    {
      _currentState = newState;
    }
    _states.at(_currentState).compBundle.AddComponents(GetOwner());
    _states.at(_currentState).transitionBlock.AddComponents(GetOwner());
  }

  bool StateComponent::IsInitialized() const
  {
    return _stateInitialized;
  }

  State &StateComponent::GetCurrentState()
  {
    return _states[_currentState];
  }
} // namespace Base
