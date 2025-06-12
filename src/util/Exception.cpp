#include "base/util/Exception.hpp"
#include <sstream>
#include <stdexcept>

namespace Base
{
  BaseException::BaseException(const std::string &message, const std::string &file, int line, const std::string &func)
    : std::runtime_error(message), file_(file), line_(line), func_(func)
  {
  }

  const char *BaseException::what() const noexcept
  {
    std::ostringstream oss;
    oss << "\033[1;31m[Runtime Error]\033[0m " << std::runtime_error::what() << "\n  \033[1;33mLocation:\033[0m "
        << file_ << ":" << line_ << "\n  \033[1;36mFunction:\033[0m " << func_ << "\n";
    formattedMessage = oss.str();
    return formattedMessage.c_str();
  }
} // namespace Base
