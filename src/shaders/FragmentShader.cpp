
#include "base/shaders/FragmentShader.hpp"
#include "base/util/Exception.hpp"
#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

namespace Base
{

  std::string FragmentShader::ReadFile(const std::filesystem::path &path)
  {
    if (!(std::filesystem::exists(path)))
    {
      std::stringstream stream;
      stream << "Shader {} does not exits " << path.string();
      THROW_BASE_RUNTIME_ERROR(stream.str());
    }

    std::fstream file;
    std::stringstream sourceStream;
    std::string sourceString;

    // vertexShader
    file.open(path);
    sourceStream << file.rdbuf();
    sourceString = sourceStream.str();
    file.close();
    return sourceString;
  }

  FragmentShader::FragmentShader(const std::filesystem::path &path) : FragmentShader(ReadFile(path))
  {
  }

  FragmentShader::FragmentShader(const std::string &source)
  {
    const char *sourceCString;
    char infoLog[512];
    int success;

    sourceCString = source.c_str();
    _id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_id, 1, &sourceCString, 0);
    glCompileShader(_id);

    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(_id, 512, 0, infoLog);
      std::cout << "Failed to compile fragment shader: " << infoLog << "\n";
    }
  }

  std::shared_ptr<FragmentShader> FragmentShader::Create(const std::filesystem::path &path)
  {
    return std::shared_ptr<FragmentShader>(new FragmentShader(path));
  }

  std::shared_ptr<FragmentShader> FragmentShader::Create(const std::string &source)
  {
    return std::shared_ptr<FragmentShader>(new FragmentShader(source));
  }

  void FragmentShader::Delete(std::shared_ptr<FragmentShader> shader)
  {
    glDeleteShader(shader->GetRenderID());
  }

  RenderID FragmentShader::GetRenderID() const
  {
    return _id;
  }

} // namespace Base
