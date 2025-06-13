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

  std::string Strip(const std::string &s)
  {
    auto start = std::find_if_not(s.begin(), s.end(), ::isspace);
    auto end = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();

    if (start >= end)
    {
      return "";
    }

    return std::string(start, end);
  }
} // namespace Base::Strings
