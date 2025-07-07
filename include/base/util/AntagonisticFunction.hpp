#pragma once
#include <functional>
namespace Base
{
  struct AntagonisticFunction
  {
    std::function<void()> flex = nullptr;
    std::function<void()> relax = nullptr;

    operator bool()
    {
      return flex && relax;
    }
  };
} // namespace Base
