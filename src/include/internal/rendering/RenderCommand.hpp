#pragma once

#include "base/assets/Font.hpp"
#include "base/camera/Camera.hpp"
#include "base/rendering/FrameBuffer.hpp"
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "base/rendering/Quad.hpp"
#include "base/rendering/Sprite.hpp"
#include "base/util/Circle.hpp"
#include "glm/ext/vector_float4.hpp"
#include <string>
#include <unordered_set>

namespace Base
{

  struct QuadCommand
  {
    Rectangle QuadToDraw;
    glm::vec3 Position;
    glm::vec4 Color;
    float Rotation;
    std::unordered_set<FramebufferAttachmentIndex> Attachments;
  };

  struct SpriteCommand
  {
    Sprite SpriteToDraw;
    glm::vec3 Position;
    glm::vec2 Size;
    float Rotation;
    std::unordered_set<FramebufferAttachmentIndex> Attachments;
  };

  struct CircleCommand
  {
    Circle Circle;
    glm::vec3 Position;
    glm::vec4 Color;
    float Thickness;
    std::unordered_set<FramebufferAttachmentIndex> Attachments;
  };

  struct TextCommand
  {
    std::wstring Text;
    std::shared_ptr<Font> FontToUse;
    glm::vec3 Position;
    glm::vec4 Color;
    float FontSize;
    std::unordered_set<FramebufferAttachmentIndex> Attachments;
  };

  struct ClearCommand
  {
    glm::vec4 Color;
    FramebufferAttachmentIndex AttachmentToClear = FramebufferAttachmentIndex::Color0;
  };

  struct SetFramebufferCommand
  {
    std::shared_ptr<FrameBuffer> FramebufferToSet = nullptr;
  };

  struct SetCameraCommand
  {
    std::shared_ptr<Camera> CameraToSet = nullptr;
  };

  using RenderCommand = std::variant<QuadCommand, SpriteCommand, CircleCommand, TextCommand, ClearCommand,
                                     SetFramebufferCommand, SetCameraCommand>;

} // namespace Base
