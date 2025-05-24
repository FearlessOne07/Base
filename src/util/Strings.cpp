#include "base/util/Strings.hpp"
#include <algorithm>
#include <string>

namespace Base::Strings
{
  std::string ToLower(const std::string &string)
  {
    std::string lower = string;
    std::ranges::transform(string, lower.begin(), [](char c) { return std::tolower(c); });
    return lower;
  }
} // namespace Base::Strings
