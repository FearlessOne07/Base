#include "base/util/UUID.hpp"
#include <cstdint>
#include <random>

namespace Base
{
  static std::random_device rd;
  static std::mt19937_64 gen(rd());
  static std::uniform_int_distribution<uint64_t> dist;

  UUID::UUID() : _uuid(dist(gen)) {};
  UUID::UUID(int id) : _uuid(id) {};

} // namespace Base
