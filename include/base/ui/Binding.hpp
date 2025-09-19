#pragma once
#include <functional>
namespace Base
{
  template <typename T> class Binding
  {
  private:
    std::function<T()> _getter = nullptr;
    std::function<void(const T &)> _setter = nullptr;

  public:
    Binding(std::function<T()> getter, std::function<void(const T &)> setter = nullptr)
      : _getter(std::move(getter)), _setter(std::move(setter))
    {
    }

    Binding() = default;
    T Get()
    {
      return _getter();
    }

    void Set(const T &value)
    {
      if (_setter)
      {
        _setter(value);
      }
    }

    operator bool() const
    {
      return static_cast<bool>(_getter);
    }
  };
} // namespace Base
