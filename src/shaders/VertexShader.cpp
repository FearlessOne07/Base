
#include "base/shaders/VertexShader.hpp"
#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <memory>

namespace Base
{

std::string VertexShader::ReadFile(const std::filesystem::path &path)
{
  if (!(std::filesystem::exists(path)))
  {
    std::stringstream stream;
    stream << "Shader {} does not exits " << path.string();
    exit(-1);
    // THROW_BASE_RUNTIME_ERROR(stream.str());
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

VertexShader::VertexShader(const std::filesystem::path &path) : VertexShader(ReadFile(path))
{
}

VertexShader::VertexShader(const std::string &source)
{
  const char *sourceCString;
  char infoLog[512];
  int success;

  sourceCString = source.c_str();
  _id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(_id, 1, &sourceCString, 0);
  glCompileShader(_id);

  glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(_id, 512, 0, infoLog);
    std::cout << "Failed to compile vertex shader: " << infoLog << "\n";
  }
}

std::shared_ptr<VertexShader> VertexShader::Create(const std::filesystem::path &path)
{
  return std::shared_ptr<VertexShader>(new VertexShader(path));
}

std::shared_ptr<VertexShader> VertexShader::Create(const std::string &source)
{
  return std::shared_ptr<VertexShader>(new VertexShader(source));
}

void VertexShader::Delete(std::shared_ptr<VertexShader> shader)
{
  glDeleteShader(shader->GetRenderID());
}

RenderID VertexShader::GetRenderID() const
{
  return _id;
}

} // namespace Base
