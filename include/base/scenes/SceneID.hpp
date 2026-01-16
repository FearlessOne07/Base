#pragma once
#include <cstdint>
#include <unordered_map>
namespace Base
{
  class SceneID
  {
  private:
    int64_t _id = -1;
    friend class SceneManager;
    explicit SceneID(int64_t id);

  public:
    SceneID();
    operator bool();
    operator int64_t() const;
    bool operator==(const SceneID &other) const;
  };
} // namespace Base

namespace std
{
  template <> struct hash<Base::SceneID>
  {
    std::size_t operator()(const Base::SceneID &id) const
    {
      return std::hash<uint64_t>()((uint64_t)id);
    }
  };
} // namespace std
