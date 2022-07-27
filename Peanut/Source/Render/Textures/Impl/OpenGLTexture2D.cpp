#if defined(PN_PLATFORM_WINDOWS)
#define _CRT_SECURE_NO_WARNINGS // fuck windows
#endif

#include "OpenGLTexture2D.hpp"

#include <Peanut/Core/Assert.hpp>
#include <stb/stb_image.h>
#include <glad/glad.h>

#include <fstream>

namespace pn {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const Texture2DSettings& settings, std::string name)
    : m_name(std::move(name))
{   
    PN_CORE_ASSERT(!settings.SizeIsExplicitlySpecified, "Unable to specify 2d texture size when loading from file");

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    PN_CORE_ENSURE(file, "Unable to open texture file {}", path);

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> fileData(size);
    if (!file.read(reinterpret_cast<char*>(&fileData[0]), size)) {
        PN_CORE_ENSURE(false, "Unable to read texture data from file {}", path);
    }
    file.close();

    stbi_set_flip_vertically_on_load(1);

    int x, y, channels;
    int desiredChannels = static_cast<int>(GetNumChannels(settings.Format));
    auto textureData = stbi_load_from_memory(&fileData[0], static_cast<int>(size), &x, &y, &channels, desiredChannels);

    if (channels < 3) {
        PN_CORE_WARN("Texture {}: number of channels in source data is < 3", m_name);
    }

    Initialize(reinterpret_cast<const void*>(textureData), {x, y}, settings);

    stbi_image_free(textureData);
}

OpenGLTexture2D::OpenGLTexture2D(const TextureData& data, const Texture2DSettings& settings, std::string name)
    : m_name(std::move(name))
{
    PN_CORE_ASSERT(settings.SizeIsExplicitlySpecified, "Size is not specified");
    
    if (!data.empty()) {
        PN_CORE_ASSERT(data.size() == settings.DesiredSize.x * settings.DesiredSize.y * GetNumChannels(settings.Format), 
            "Texture data size doesn't equal x * y * channels");
    }

    const void* ptr = data.empty() ? nullptr : reinterpret_cast<const void*>(&data[0]);
    Initialize(ptr, settings.DesiredSize, settings);
}

void OpenGLTexture2D::Initialize(const void* data, const glm::u32vec2& size, const Texture2DSettings& settings)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_descriptor);

    m_size = size;

    m_format = ToGlFormat(settings.Format);
    m_numChannels = GetNumChannels(settings.Format);

    m_useMipmaps = settings.UseMipmaps;

    int numTextuteLevels = m_useMipmaps ? GetNumTextureLevels(m_size) : 1;
    glTextureStorage2D(m_descriptor, numTextuteLevels, ToGlInternalFormat(settings.Format), m_size.x, m_size.y);

    if (data) {
        glTextureSubImage2D(m_descriptor, 0, 0, 0, m_size.x, m_size.y, m_format, GL_UNSIGNED_BYTE, data);
        if (m_useMipmaps) {
            glGenerateTextureMipmap(m_descriptor);
        }
    }

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
    glBindTextureUnit(slot, m_descriptor);
}

void OpenGLTexture2D::Unbind()
{
    glBindTextureUnit(m_slot, m_descriptor);
}

void OpenGLTexture2D::SetData(const TextureData& data)
{
    PN_CORE_ASSERT(!data.empty(), "No data to set");
    PN_CORE_ASSERT(data.size() == m_size.x * m_size.y * m_numChannels, "Provided data size doesn't equal to texture buffer size");

    glTextureSubImage2D(m_descriptor, 0, 0, 0, m_size.x, m_size.y, m_format, GL_UNSIGNED_BYTE, &data[0]);
    if (m_useMipmaps) {
        glGenerateTextureMipmap(m_descriptor);
    }
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

int OpenGLTexture2D::ToGlInternalFormat(TextureFormat format) const
{
    switch (format) {
        case TextureFormat::RGB:    return GL_RGB8;
        case TextureFormat::RGBA:   return GL_RGBA8;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
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

uint32_t OpenGLTexture2D::GetNumChannels(TextureFormat format) const
{
    switch (format) {
        case TextureFormat::RGB:    return 3u;
        case TextureFormat::RGBA:   return 4u;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
    return 0u;
}

uint32_t OpenGLTexture2D::GetNumTextureLevels(const glm::u32vec2& textureSize) const 
{
    return 1u + static_cast<uint32_t>(std::floor(std::log2f(static_cast<float>(std::max(textureSize.x, textureSize.y)))));
}


}
