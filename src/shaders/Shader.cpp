#include "base/shaders/Shader.hpp"
#include "base/rendering/GeometryType.hpp"
#include "glad/glad.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>

namespace Base
{

  Shader::Shader( //
    const std::shared_ptr<VertexShader> &vertex, const std::shared_ptr<FragmentShader> &fragment,
    GeometryType type //
    )
    : _type(type)
  {
    RenderID vertexID = vertex->GetRenderID();
    RenderID fragID = fragment->GetRenderID();

    int success = 0;
    char infoLog[512];
    _id = glCreateProgram();
    glAttachShader(_id, vertexID);
    glAttachShader(_id, fragID);
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(_id, sizeof(infoLog), 0, infoLog);
      std::cout << "Failed to link program: " << infoLog << "\n";
    }

    FragmentShader::Delete(fragment);
    VertexShader::Delete(vertex);
  }

  std::shared_ptr<Shader> Shader::Create( //
    const std::filesystem::path &vertex, const std::filesystem::path &fragment,
    GeometryType shaderClass //
  )
  {
    if (std::filesystem::exists(vertex) && std::filesystem::exists(fragment))
    {
      return std::shared_ptr<Shader>(
        new Shader(VertexShader::Create(vertex), FragmentShader::Create(fragment), shaderClass));
    }
    else if (vertex.empty() || fragment.empty())
    {
      if (vertex.empty() && fragment.empty())
      {
        return std::shared_ptr<Shader>( //
          new Shader(                   //
            VertexShader::Create(_defaultVertexShaders[shaderClass]),
            FragmentShader::Create(_defaultFragmentShaders[shaderClass]), shaderClass //
            )                                                                         //
        );
      }
      else if (vertex.empty())
      {
        return std::shared_ptr<Shader>(new Shader(VertexShader::Create(_defaultVertexShaders[shaderClass]),
                                                  FragmentShader::Create(fragment), shaderClass) //
        );
      }
      else if (fragment.empty())
      {
        return std::shared_ptr<Shader>(                                                                             //
          new Shader(                                                                                               //
            VertexShader::Create(vertex), FragmentShader::Create(_defaultFragmentShaders[shaderClass]), shaderClass //
            )                                                                                                       //
        );
      }
    }
    exit(-1);
  }

  void Shader::SetMaxTextureSlots(uint8_t max)
  {
    _maxTextureSlots = max;
    _defaultFragmentShaders = {
      {
        GeometryType::Quad,
        std::format(
          R"(
        #version 460
        
        // Inputsshader
        in vec4 vFragColor;
        in vec3 vUv;
        flat in uint vAttachmentMask;
        
        // Outputs
        layout(location = 0) out vec4 oColor0;
        layout(location = 1) out vec4 oColor1;
        layout(location = 2) out vec4 oColor2;
        layout(location = 3) out vec4 oColor3;
        layout(location = 4) out vec4 oColor4;
        layout(location = 5) out vec4 oColor5;
        layout(location = 6) out vec4 oColor6;
        layout(location = 7) out vec4 oColor7;
        
        layout(binding = 0) uniform sampler2D textures[{0}];
        
        void main() {{
          vec4 texel = texture(textures[int(vUv.z)], vUv.xy) * vFragColor;

          if(texel.a <= 0.0){{
            discard;
          }}
          texel.rgb *= texel.a;

          vec4 color = texel;
          
          oColor0 = ((vAttachmentMask & 1u) != 0u) ? color : vec4(0.0);
          oColor1 = ((vAttachmentMask & 2u) != 0u) ? color : vec4(0.0);
          oColor2 = ((vAttachmentMask & 4u) != 0u) ? color : vec4(0.0);
          oColor3 = ((vAttachmentMask & 8u) != 0u) ? color : vec4(0.0);
          oColor4 = ((vAttachmentMask & 16u) != 0u) ? color : vec4(0.0);
          oColor5 = ((vAttachmentMask & 32u) != 0u) ? color : vec4(0.0);
          oColor6 = ((vAttachmentMask & 64u) != 0u) ? color : vec4(0.0);
          oColor7 = ((vAttachmentMask & 128u) != 0u) ? color : vec4(0.0);
        }}
       )",
          _maxTextureSlots),
      },

      {
        GeometryType::Text,
        R"(
       #version 460
       
       // Inputs
       in vec4 vFragColor;
       in vec3 vUv;
       flat in uint vAttachmentMask;
       
       // Outputs
       layout(location = 0) out vec4 oColor0;
       layout(location = 1) out vec4 oColor1;
       layout(location = 2) out vec4 oColor2;
       layout(location = 3) out vec4 oColor3;
       layout(location = 4) out vec4 oColor4;
       layout(location = 5) out vec4 oColor5;
       layout(location = 6) out vec4 oColor6;
       layout(location = 7) out vec4 oColor7;
       
       layout(binding = 0) uniform sampler2D texture0;
       
       void main() {
          float sdf = texture(texture0, vUv.xy).r;
          float edgeWidth = fwidth(sdf);
          float alpha = smoothstep(0.5 - edgeWidth, 0.5 + edgeWidth, sdf);
          
        
          vec4 color = vFragColor;
          color.a = alpha * vFragColor.a;

          if(color.a <= 0){
            discard;
          }

          color.rgb *= color.a;
          
          oColor0 = ((vAttachmentMask & 1u) != 0u) ? color : vec4(0.0);
          oColor1 = ((vAttachmentMask & 2u) != 0u) ? color : vec4(0.0);
          oColor2 = ((vAttachmentMask & 4u) != 0u) ? color : vec4(0.0);
          oColor3 = ((vAttachmentMask & 8u) != 0u) ? color : vec4(0.0);
          oColor4 = ((vAttachmentMask & 16u) != 0u) ? color : vec4(0.0);
          oColor5 = ((vAttachmentMask & 32u) != 0u) ? color : vec4(0.0);
          oColor6 = ((vAttachmentMask & 64u) != 0u) ? color : vec4(0.0);
          oColor7 = ((vAttachmentMask & 128u) != 0u) ? color : vec4(0.0);
       }
      )",
      },
      {
        GeometryType::Circle,
        R"(
          #version 460
          
          // Inputs
          in vec4 vFragColor;
          in vec2 vUv;
          in flat uint vAttachmentMask;
          in flat vec2 vThicknessFade;
          
          // Outputs
          layout(location = 0) out vec4 oColor0;
          layout(location = 1) out vec4 oColor1;
          layout(location = 2) out vec4 oColor2;
          layout(location = 3) out vec4 oColor3;
          layout(location = 4) out vec4 oColor4;
          layout(location = 5) out vec4 oColor5;
          layout(location = 6) out vec4 oColor6;
          layout(location = 7) out vec4 oColor7;
          
          void main() {
          {
            vec4 color = vec4(1.0);
            vec2 uv = vUv * 2.0 - 1.0;
            float fade = vThicknessFade.y;
            float thickness = vThicknessFade.x;
          
            float distance = 1.0 - length(uv);
          
            float circleAlpha = smoothstep(0.0, fade,distance);
            circleAlpha*= smoothstep(thickness + fade, thickness,distance);
          
            color = vFragColor;
            color.a = vFragColor.a * circleAlpha;

            if(color.a <= 0.0){
              discard;
            }

            color.rgb *= color.a;
  
            oColor0 = ((vAttachmentMask & 1u) != 0u) ? color : vec4(0.0);
            oColor1 = ((vAttachmentMask & 2u) != 0u) ? color : vec4(0.0);
            oColor2 = ((vAttachmentMask & 4u) != 0u) ? color : vec4(0.0);
            oColor3 = ((vAttachmentMask & 8u) != 0u) ? color : vec4(0.0);
            oColor4 = ((vAttachmentMask & 16u) != 0u) ? color : vec4(0.0);
            oColor5 = ((vAttachmentMask & 32u) != 0u) ? color : vec4(0.0);
            oColor6 = ((vAttachmentMask & 64u) != 0u) ? color : vec4(0.0);
            oColor7 = ((vAttachmentMask & 128u) != 0u) ? color : vec4(0.0);
          }
        }
      )",
      },
    };
  }

  void Shader::Delete(std::shared_ptr<Shader> shader)
  {
    glDeleteShader(shader->_id);
  }

  void Shader::Use()
  {
    glUseProgram(_id);
    ApplyPendingUniforms();
  }
  void Shader::Unuse() const
  {
    glUseProgram(0);
  }

  void Shader::SetUniform(                             //
    const std::string &name, const UniformValue &value //
  )
  {
    _pendingUniforms.insert_or_assign(name, value);
  }

  void Shader::ApplyPendingUniforms()
  {
    for (auto &[name, value] : _pendingUniforms)
    {
      int loc;
      if (_uniformLocationCache.contains(name))
      {
        loc = _uniformLocationCache.at(name);
      }
      else
      {
        loc = glGetUniformLocation(_id, name.c_str());
        _uniformLocationCache.insert({name, loc});
      }

      if (std::holds_alternative<float>(value))
      {
        float v = std::get<float>(value);
        glUniform1f(loc, v);
      }
      else if (std::holds_alternative<glm::vec2>(value))
      {
        glm::vec2 v = std::get<glm::vec2>(value);
        glUniform2fv(loc, 1, glm::value_ptr(v));
      }
      else if (std::holds_alternative<glm::vec3>(value))
      {
        glm::vec3 v = std::get<glm::vec3>(value);
        glUniform3fv(loc, 1, glm::value_ptr(v));
      }
      else if (std::holds_alternative<glm::vec4>(value) || std::holds_alternative<Color>(value))
      {
        glm::vec4 v;
        if (std::holds_alternative<glm::vec4>(value))
        {
          v = std::get<glm::vec4>(value);
        }
        else
        {
          Color c = std::get<Color>(value);
          v = {c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f};
        }
        glUniform4fv(loc, 1, glm::value_ptr(v));
      }
      else if (std::holds_alternative<int>(value))
      {
        int v = std::get<int>(value);
        glUniform1i(loc, v);
      }
      else if (std::holds_alternative<glm::mat4>(value))
      {
        glm::mat4 v = std::get<glm::mat4>(value);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
      }
    }
  }

  GeometryType Shader::GetClass() const
  {
    return _type;
  }
} // namespace Base
