#include "OpenGLTexture2D.hpp"

#include <Peanut/Core/Assert.hpp>
#include <stb/stb_image.h>
#include <glad/glad.h>

#include <cstdio>

namespace pn {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const Texture2DSettings& settings, std::string name)
    : m_name(std::move(name))
{   
    FILE* f = fopen(path.c_str(), "r");
    PN_CORE_ASSERT(f, "Unable to open texture file {}", path);

    size_t fileSize = GetFileSize(f);
    TextureData data;
    data.bytes.resize(fileSize);
    fread(&data.bytes[0], fileSize, 1, f);

    fclose(f);
    
    InitializeFromData(data, settings);
}

size_t OpenGLTexture2D::GetFileSize(FILE* f)
{
    long prevPos = ftell(f);
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, prevPos, SEEK_SET);
    return size;
}

OpenGLTexture2D::OpenGLTexture2D(const TextureData& data, const Texture2DSettings& settings, std::string name)
    : m_name(std::move(name))
{
    InitializeFromData(data, settings);
}

void OpenGLTexture2D::InitializeFromData(const TextureData& data, const Texture2DSettings& settings)
{
    int len = static_cast<int>(data.bytes.size());

    int x, y, channels;
    stbi_set_flip_vertically_on_load(1);
    int desiredChannels = GetNumChannels(settings.Format);
    unsigned char* texData = stbi_load_from_memory(&data.bytes[0], len, &x, &y, &channels, desiredChannels);

    if (channels < 3) {
        PN_CORE_WARN("Texture {}: number of channels in source data is < 3", m_name);
    }

    m_x = x;
    m_y = y;

    glGenTextures(1, &m_descriptor);
    glBindTexture(GL_TEXTURE_2D, m_descriptor);

    GLint format = ToGlFormat(settings.Format);
    glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, texData);

    if (settings.UseMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(texData);

    SetWrapping(settings.WrapX, settings.WrapY);

    if (settings.UseMipmaps) {
        SetFiltering(settings.MipmapFilterMin, settings.FilterMag);
    } else {
        SetFiltering(settings.FilterMin, settings.FilterMag);
    }
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_descriptor);
}

void OpenGLTexture2D::BindToSlot(uint32_t slot)
{
    m_slot = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_descriptor);
}

void OpenGLTexture2D::Unbind()
{
    glActiveTexture(GL_TEXTURE0 + m_slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture2D::SetWrapping(TextureWrap x, TextureWrap y)
{
    glBindTexture(GL_TEXTURE_2D, m_descriptor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGlWrap(x));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGlWrap(y));
}

void OpenGLTexture2D::SetFiltering(TextureFilter min, TextureFilter mag)
{
    glBindTexture(GL_TEXTURE_2D, m_descriptor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGlFilter(min));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGlFilter(mag));
}

void OpenGLTexture2D::SetFiltering(TextureMipmapFilter min, TextureFilter mag)
{
    glBindTexture(GL_TEXTURE_2D, m_descriptor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGlFilter(min));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGlFilter(mag));
}

int OpenGLTexture2D::ToGlWrap(TextureWrap wrap) const
{
    switch (wrap) {
        case TextureWrap::Repeat:           return GL_REPEAT;
        case TextureWrap::MirroredRepeat:   return GL_MIRRORED_REPEAT;
        case TextureWrap::ClampToBorder:    return GL_CLAMP_TO_BORDER;
        case TextureWrap::ClampToEdge:      return GL_CLAMP_TO_EDGE;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown TextureWrap enum value");
    return 0;
}

int OpenGLTexture2D::ToGlFilter(TextureFilter filter) const
{
    switch (filter) {
        case TextureFilter::Nearest: return GL_NEAREST;
        case TextureFilter::Linear: return  GL_LINEAR;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown TextureFilter enum value");
    return 0;
}

int OpenGLTexture2D::ToGlFilter(TextureMipmapFilter filter) const
{
    switch (filter) {
        case TextureMipmapFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
        case TextureMipmapFilter::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
        case TextureMipmapFilter::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
        case TextureMipmapFilter::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown TextureMipmapFilter enum value");
    return 0;
}

int OpenGLTexture2D::ToGlFormat(TextureFormat format) const
{
    switch (format) {
        case TextureFormat::RGB:    return GL_RGB;
        case TextureFormat::RGBA:   return GL_RGBA;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
    return 0;
}

int OpenGLTexture2D::GetNumChannels(TextureFormat format) const
{
    switch (format) {
        case TextureFormat::RGB:    return 3;
        case TextureFormat::RGBA:   return 4;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
    return 0;
}

}
