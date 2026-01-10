#pragma once
#include "FragmentShader.hpp"
#include "VertexShader.hpp"
#include "base/rendering/GeometryType.hpp"
#include "base/rendering/RenderID.hpp"
#include "base/shaders/UniformValue.hpp"
#include <filesystem>
#include <memory>
#include <unordered_map>

namespace Base
{
  class Shader
  {
  private:
    RenderID _id;
    GeometryType _type = GeometryType::Quad;
    std::unordered_map<std::string, UniformValue> _pendingUniforms;
    std::unordered_map<std::string, int32_t> _uniformLocationCache;

    inline static uint8_t _maxTextureSlots = 0;
    inline static std::unordered_map<GeometryType, const std::string> _defaultVertexShaders = {
      {
        GeometryType::Quad,
        R"(
        #version 460
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aUv;
        layout(location = 2) in vec4 aColor;
        layout(location = 3) in uint aAttachmentMask;
        
        //Outputs
        out vec4 vFragColor;
        out vec3 vUv;
        out flat uint vAttachmentMask;
        
        // Uniforms
        uniform mat4 uProjView;
        
        void main() {
          gl_Position = uProjView * vec4(aPosition, 1.0);
          vFragColor = aColor;
          vUv = aUv;
          vAttachmentMask = aAttachmentMask;
        }
      )",
      },
      {
        GeometryType::Text,
        R"(
        #version 460
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aUv;
        layout(location = 2) in vec4 aColor;
        layout(location = 3) in uint aAttachmentMask;
        
        //Outputs
        out vec4 vFragColor;
        out vec3 vUv;
        out flat uint vAttachmentMask;
        
        // Uniforms
        uniform mat4 uProjView;
        
        void main() {
          gl_Position = uProjView * vec4(aPosition, 1.0);
          vFragColor = aColor;
          vUv = aUv;
          vAttachmentMask = aAttachmentMask;
        }
      )",
      },
      {
        GeometryType::Circle,
        R"(        
        #version 460
        
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec4 aColor;
        layout(location = 2) in vec2 aUv;
        layout(location = 3) in vec2 aThicknessFade;
        layout(location = 4) in uint aAttachmentMask;
        
        //Outputs
        out vec4 vFragColor;
        out vec2 vUv;
        out flat uint vAttachmentMask;
        out flat vec2 vThicknessFade;
        
        // Uniforms
        uniform mat4 uProjView;
        
        void main() {
          gl_Position = uProjView * vec4(aPosition, 1.0);
          vFragColor = aColor;
          vUv = aUv;
          vAttachmentMask = aAttachmentMask;
          vThicknessFade = aThicknessFade;
        }
      )",
      },
    };

    inline static std::unordered_map<GeometryType, const std::string> _defaultFragmentShaders;

  private:
    Shader(const std::shared_ptr<VertexShader> &vertex, const std::shared_ptr<FragmentShader> &fragment);
    void ApplyPendingUniforms();

  public:
    static std::shared_ptr<Shader> Create( //
      const std::filesystem::path &vertex, const std::filesystem::path &fragment,
      GeometryType shaderClass //
    );
    static void Delete(std::shared_ptr<Shader> shader);
    static void SetMaxTextureSlots(uint8_t);
    Shader() = default;
    void Use();
    void Unuse() const;

    // Set Uniform
    void SetUniform(const std::string &name, const UniformValue &value);
    GeometryType GetClass() const;
  };

} // namespace Base
