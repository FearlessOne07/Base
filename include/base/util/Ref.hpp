#pragma once

#include <functional>
#include <optional>
namespace Base
{
  template <typename T> class Ref
  {
  private:
    std::optional<std::reference_wrapper<T>> _ref;

  public:
    Ref() = default;
    Ref(T &object) : _ref(object) {};

    Ref(std::optional<std::reference_wrapper<T>> value) : _ref(value) {};

    bool HasValue() const
    {
      return _ref.has_value();
    }

    operator bool() const
    {
      return HasValue();
    }

    T &Get()
    {
      return _ref->get();
    }

    const T &Get() const
    {
      return _ref->get();
    }

    T *operator->()
    {
      return &Get();
    }

    const T *operator->() const
    {
      return &Get();
    }

    // Allow implicit conversion from Ref<T> -> Ref<const T>
    template <typename U = T> operator Ref<const U>() const
    {
      static_assert(!std::is_const_v<U>, "Cannot convert Ref<const T> to Ref<const T> again");
      return Ref<const U>(_ref);
    }
  };
  template <typename T> using ConstRef = Ref<const T>;
} // namespace Base
