#pragma once
#include "base/components/Component.hpp"
#include "base/components/ComponentBundle.hpp"
#include "base/state/TransitionConditionBlock.hpp"
#include <string>
#include <unordered_map>
namespace Base
{
  struct State
  {
    ComponentBundle compBundle;
    TransitionConditionBlock transitionBlock;
  };

  class StateComponent : public Component
  {
  private:
    std::string _currentState = "";
    std::string _initialState = "";
    std::unordered_map<std::string, State> _states;
    bool _stateInitialized = false;

  public:
    StateComponent(const std::string &initialState, const std::unordered_map<std::string, State> &states);
    void SwitchState(const std::string &newState, bool initialize = false);
    bool IsInitialized() const;
    State &GetCurrentState();
  };
} // namespace Base
