#pragma once
#include <stdexcept>

namespace Base
{
  class BaseException : public std::runtime_error
  {
  public:
    BaseException(const std::string &message, const std::string &file, int line, const std::string &func);

    const char *what() const noexcept override;

  private:
    std::string file_;
    int line_;
    std::string func_;
    mutable std::string formattedMessage; // mutable to allow caching in const what()
  };

#define THROW_BASE_RUNTIME_ERROR(msg) throw BaseException((msg), __FILE__, __LINE__, __func__)
} // namespace Base
//
