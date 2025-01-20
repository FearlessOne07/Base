#include "base/Exports.hpp"
#include <memory>
#include <unordered_map>

namespace Base
{

  class System;
  class BASEAPI SceneManager
  {
  private:
    std::unordered_map<unsigned int, std::unique_ptr<System>> _systems;

  public:
    void RegisterSystem();
  };
} // namespace Base
