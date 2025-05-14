#include <functional>
#include <optional>
namespace Base
{
  struct AntagonisticFunction
  {
    std::function<void()> agonist = nullptr;
    std::function<void()> antagonist = nullptr;

    operator bool()
    {
      return !agonist && !antagonist;
    }
  };
} // namespace Base
