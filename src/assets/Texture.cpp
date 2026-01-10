#include "base/assets/Texture.hpp"
#include "glad/glad.h"
#include <filesystem>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Base
{

  static GLenum GetGLInternalFormat(TextureFormat format)
  {
    switch (format)
    {
    case TextureFormat::RGBA8:
      return GL_RGBA8;
    case TextureFormat::RGB8:
      return GL_RGB8;
    case TextureFormat::R8:
      return GL_R8;
    case TextureFormat::RGB16F:
      return GL_RGB16F;
    case TextureFormat::RGB32F:
      return GL_RGB32F;
    }
    return 0;
  }

  static GLenum GetGLExternalFormat(TextureFormat format)
  {
    switch (format)
    {
    case TextureFormat::RGBA8:
      return GL_RGBA;
    case TextureFormat::RGB8:
      return GL_RGB;
    case TextureFormat::R8:
      return GL_RED;
    case TextureFormat::RGB16F:
    case TextureFormat::RGB32F:
      return GL_RGB;
    }
    return 0;
  }

  static GLenum GetGLDataType(TextureFormat format)
  {
    switch (format)
    {
    case TextureFormat::RGBA8:
    case TextureFormat::RGB8:
    case TextureFormat::R8:
      return GL_UNSIGNED_BYTE;
    case TextureFormat::RGB16F:
    case TextureFormat::RGB32F:
      return GL_FLOAT;
    }
    return 0;
  }

  static GLenum GetGLFilterType(TextureFiltering filter, bool MipMaps = false)
  {
    switch (filter)
    {
    case TextureFiltering::Nearest:
      if (MipMaps)
      {
        return GL_NEAREST_MIPMAP_NEAREST;
      }
      else
      {
        return GL_NEAREST;
      }
    case TextureFiltering::Linear:
      if (MipMaps)
      {
        return GL_LINEAR_MIPMAP_LINEAR;
      }
      else
      {
        return GL_LINEAR;
      }
      break;
    }
    return 0;
  }

  static int CalculateMipLevels(int w, int h)
  {
    return 1 + (int)std::floor(std::log2(std::max(w, h)));
  }

  std::shared_ptr<Texture> Texture::Create(const std::filesystem::path &path, const TextureSpec &spec)
  {
    return std::shared_ptr<Texture>(new Texture(path, spec));
  }

  std::shared_ptr<Texture> Texture::Create(void *data, const TextureSpec &spec)
  {
    return std::shared_ptr<Texture>(new Texture(data, spec));
  }

  void Texture::Destroy(std::shared_ptr<Texture> texture)
  {
    glDeleteTextures(1, texture->_id.data());
  }

  Texture::Texture(const std::filesystem::path &path, const TextureSpec &spec) : _spec(spec)
  {
    if (!std::filesystem::exists(path))
    {
      exit(-1);
    }

    stbi_set_flip_vertically_on_load(true);

    int channels;
    unsigned char *data = stbi_load(path.string().c_str(), &_spec.Width, &_spec.Height, &channels, 4);

    if (!data)
    {
      exit(-1);
    }

    glGenTextures(1, _id.data());
    glBindTexture(GL_TEXTURE_2D, _id);

    int mipLevels = spec.MipMaps ? CalculateMipLevels(_spec.Width, _spec.Height) : 1;
    glTexStorage2D(GL_TEXTURE_2D, mipLevels, GetGLInternalFormat(spec.Format), _spec.Width, _spec.Height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _spec.Width, _spec.Height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (spec.MipMaps)
    {
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterType(spec.MinFilter, spec.MipMaps));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterType(spec.MagFilter));

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
  }

  Texture::Texture(void *data, const TextureSpec &spec) : _spec(spec)
  {
    glGenTextures(1, _id.data());
    glBindTexture(GL_TEXTURE_2D, _id);

    const int mipLevels = spec.MipMaps ? CalculateMipLevels(_spec.Width, _spec.Height) : 1;

    glTexStorage2D(GL_TEXTURE_2D, mipLevels, GetGLInternalFormat(spec.Format), _spec.Width, _spec.Height);

    if (data != nullptr)
    {
      glTexSubImage2D( //
        GL_TEXTURE_2D, 0, 0, 0, _spec.Width, _spec.Height, GetGLExternalFormat(spec.Format), GetGLDataType(spec.Format),
        data //
      );

      if (spec.MipMaps)
      {
        glGenerateMipmap(GL_TEXTURE_2D);
      }
    }

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterType(spec.MinFilter, spec.MipMaps));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterType(spec.MagFilter));

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Texture::Bind(int slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _id);
  }

  int Texture::GetWidth() const
  {
    return _spec.Width;
  }

  int Texture::GetHeight() const
  {
    return _spec.Height;
  }

  RenderID Texture::GetRenderID() const
  {
    return _id;
  }

} // namespace Base
