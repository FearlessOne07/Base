#include "base/Exports.hpp"
#include "base/System.hpp"
#include <unordered_map>

namespace Base
{
  class BASEAPI SceneManager
  {
  private:
    std::unordered_map<unsigned int, System> _systems;

  public:
    void RegisterSystem();
  };
} // namespace Base
