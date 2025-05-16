#include <string>

namespace Base
{
  struct TweenKey
  {
    const void *objectPtr;
    std::string property;

    bool operator==(const TweenKey &other) const
    {
      return objectPtr == other.objectPtr && property == other.property;
    }
  };
} // namespace Base

namespace std
{
  template <> struct hash<Base::TweenKey>
  {
    std::size_t operator()(const Base::TweenKey &key) const
    {
      return std::hash<const void *>()(key.objectPtr) ^ std::hash<std::string>()(key.property);
    }
  };
} // namespace std
