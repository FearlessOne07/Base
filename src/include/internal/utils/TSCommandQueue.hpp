#pragma once

#include <array>
#include <atomic>

namespace Base
{
  template <typename T, size_t Size> class TSCommandQueue
  {
  private:
    struct Command
    {
      T data;
      std::atomic<bool> ready{false};
    };

    std::array<Command, Size> _commands;
    std::atomic<size_t> _writeIndex{0};
    std::atomic<size_t> _readIndex{0};

  public:
    bool Push(const T &item)
    {
      size_t write = _writeIndex.load(std::memory_order_relaxed);
      size_t nextWrite = (write + 1) % Size;

      if (nextWrite == _readIndex.load(std::memory_order_acquire))
      {
        return false;
      }

      _commands[write].data = item;
      _commands[write].ready.store(true, std::memory_order_release);
      _writeIndex.store(nextWrite, std::memory_order_release);
      return true;
    }

    bool Pop(T &item)
    {
      size_t read = _readIndex.load(std::memory_order_relaxed);

      if (!_commands[read].ready.load(std::memory_order_acquire))
      {
        return false;
      }

      item = _commands[read].data;
      _commands[read].ready.store(false, std::memory_order_release);
      _readIndex.store((read + 1) % Size, std::memory_order_release);
      return true;
    }
  };
} // namespace Base
