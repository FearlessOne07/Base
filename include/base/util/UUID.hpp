#pragma once
#include <cstdint>
#include <unordered_map>
namespace Base
{
  class UUID
  {
  private:
    uint64_t _uuid;

  public:
    UUID();
    UUID(const UUID &) = default;
    operator uint64_t() const
    {
      return _uuid;
    }
  };
} // namespace Base

namespace std
{
  template <> struct hash<Base::UUID>
  {
    std::size_t operator()(const Base::UUID &uuid) const
    {
      return std::hash<uint64_t>()((uint64_t)uuid);
    }
  };
} // namespace std
