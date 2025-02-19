#include "base/RenderContextSingleton.hpp"
#include "base/RenderContext.hpp"

namespace Base
{
  RenderContext *RenderContextSingleton::_instance = nullptr;

  const RenderContext *RenderContextSingleton::GetInstance()
  {
    return _instance;
  }

  void RenderContextSingleton::UpdateInstance(RenderContext *newContext)
  {
    _instance = newContext;
  }

  // Destructor: cleans up the instance if it exists
  RenderContextSingleton::~RenderContextSingleton()
  {
    delete _instance; // Clean up the instance if it was allocated
    _instance = nullptr;
  }
} // namespace Base
