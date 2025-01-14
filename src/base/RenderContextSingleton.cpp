#include "base/RenderContextSingleton.hpp"
#include "base/RenderContext.hpp"

namespace Base
{
  RenderContext RenderContextSingleton::_instance = {};

  const RenderContext *RenderContextSingleton::GetInstance()
  {
    return &_instance;
  }

  void RenderContextSingleton::UpdateInstance(const RenderContext *newContext)
  {
    _instance = *newContext;
  }
} // namespace Base
