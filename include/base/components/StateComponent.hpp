#pragma once
#include "base/components/Component.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace Base
{
  class StateComponent : public Component
  {
    std::string _currentState = "";

    std::unordered_map<std::string, std::unique_ptr<IComponentBundle>> _states;
  };
} // namespace Base
//
