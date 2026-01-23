#pragma once
#include "base/rendering/RenderID.hpp"
#include <filesystem>
#include <string>

namespace Base
{

  class FragmentShader
  {
  private:
    RenderID _id;

  private:
    FragmentShader(const std::filesystem::path &path);
    FragmentShader(const std::string &source);
    static std::string ReadFile(const std::filesystem::path &path);

  public:
    static std::shared_ptr<FragmentShader> Create(const std::filesystem::path &path);
    static std::shared_ptr<FragmentShader> Create(const std::string &source);
    static void Delete(std::shared_ptr<FragmentShader> shader);
    RenderID GetRenderID() const;
    FragmentShader() = default;
  };

} // namespace Base
