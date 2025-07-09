#pragma once
#include "base/util/UUID.hpp"
#include <cstddef>
#include <functional>
#include <memory>

namespace Base
{
  template <typename T> class AssetHandle
  {

    template <typename> friend class AssetHandle;

  private:
    UUID _uuid = UUID(0);
    std::weak_ptr<T> _assetPointer;

  public:
    static AssetHandle<T> Cast(AssetHandle<void> &handle)
    {
      auto newHandle = AssetHandle<T>(std::static_pointer_cast<T>(handle._assetPointer.lock()));
      newHandle._uuid = handle._uuid;
      return newHandle;
    }

    static const AssetHandle<T> Cast(const AssetHandle<void> &handle)
    {
      auto newHandle = AssetHandle<T>(std::static_pointer_cast<T>(handle._assetPointer.lock()));
      newHandle._uuid = handle._uuid;
      return newHandle;
    }

    AssetHandle(std::weak_ptr<T> ptr) : _assetPointer(ptr)
    {
      _uuid = UUID();
    };

    AssetHandle(std::shared_ptr<T> ptr) : _assetPointer(ptr)
    {
      _uuid = UUID();
    };

    AssetHandle() {};
    inline std::shared_ptr<T> Get() const
    {
      return _assetPointer.lock();
    }

    inline UUID GetUUID() const
    {
      return _uuid;
    }

    // Conversion to AssetHandle<void>
    operator AssetHandle<void>() const
    {
      auto handle = AssetHandle<void>(std::static_pointer_cast<void>(_assetPointer.lock()));
      handle._uuid = _uuid;
      return handle;
    }

    inline bool operator==(const AssetHandle<T> &other) const
    {
      return other._uuid == _uuid;
    }

    inline operator bool()
    {
      return _uuid != 0;
    }
  };
} // namespace Base
//

namespace std
{
  template <typename T> struct hash<Base::AssetHandle<T>>
  {
    std::size_t operator()(const Base::AssetHandle<T> &handle) const noexcept
    {
      return std::hash<Base::UUID>{}(handle.GetUUID());
    }
  };
} // namespace std
