#pragma once
#include "base/rendering/RenderID.hpp"
#include <filesystem>
#include <string>

namespace Base
{
  class VertexShader
  {
  private:
    RenderID _id;

  private:
    VertexShader(const std::filesystem::path &path);
    VertexShader(const std::string &source);
    static std::string ReadFile(const std::filesystem::path &path);

  public:
    static std::shared_ptr<VertexShader> Create(const std::filesystem::path &path);
    static std::shared_ptr<VertexShader> Create(const std::string &source);
    static void Delete(std::shared_ptr<VertexShader> shader);
    RenderID GetRenderID() const;
    VertexShader() = default;
  };
} // namespace Base
