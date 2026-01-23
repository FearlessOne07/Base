#pragma once
#include "base/camera/Camera.hpp"
#include "base/rendering/GeometryType.hpp"
#include "base/shaders/Shader.hpp"
#include "internal/rendering/IndexBuffer.hpp"
#include "internal/rendering/RenderCommand.hpp"
#include "internal/rendering/VertexArray.hpp"
#include "internal/rendering/VertexBuffer.hpp"
#include <cstdint>
#include <memory>
#include <vector>

namespace Base
{

  class Batcher
  {
  protected:
    Ptr<VertexArray> _vao;
    Ptr<VertexBuffer> _vbo;
    Ptr<IndexBuffer> _ibo;
    std::vector<uint16_t> _indices;
    uint64_t _currentIndex = 0;

    std::shared_ptr<Shader> _defaultShader = nullptr;
    std::shared_ptr<Shader> _currentShader = nullptr;
    std::shared_ptr<Camera> _currentCamera = nullptr;

    GeometryType _geometryType;

  protected:
    virtual void Init() = 0;
    virtual void DeInit() = 0;

  public:
    virtual void Begin() = 0;
    virtual void Flush() = 0;
    virtual void Submit(const RenderCommand &command) = 0;
    void ResetShader()
    {
      _currentShader = _defaultShader;
    }

    GeometryType GetGeometryType() const
    {
      return _geometryType;
    }

    void SetCamera(std::shared_ptr<Camera> camera)
    {
      _currentCamera = camera;
    }

    void SetShader(std::shared_ptr<Shader> shader)
    {
      _currentShader = shader;
    }

    virtual ~Batcher()
    {
    }
  };

} // namespace Base
