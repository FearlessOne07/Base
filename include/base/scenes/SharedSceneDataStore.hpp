#pragma once
#include <memory>
namespace Base
{
  template <typename T> class SharedSceneDataStore
  {
    template <typename> friend class SharedSceneDataStore;

  private:
    std::shared_ptr<T> _data = nullptr;

  public:
    void Init()
    {
      _data = std::make_shared<T>();
    }

    template <typename U> std::shared_ptr<U> Get()
    {
      return std::static_pointer_cast<U>(_data);
    }

    operator bool()
    {
      return _data;
    }

    operator SharedSceneDataStore<void>() const
    {
      SharedSceneDataStore<void> data;
      data._data = std::static_pointer_cast<void>(_data);
      return data;
    }
  };
} // namespace Base
