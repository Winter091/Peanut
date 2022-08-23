#if defined(PN_PLATFORM_WINDOWS)
#define _CRT_SECURE_NO_WARNINGS // fuck windows
#endif

#include "OpenGLTexture2D.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Peanut/Core/Utils/FileUtils.hpp>
#include <Render/Textures/Impl/OpenGLTextureSampler.hpp>
#include <Render/Textures/TextureLoad.hpp>

#include <stb/stb_image.h>
#include <glad/glad.h>

#include <algorithm>

namespace pn {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const Texture2DSettings& settings, std::string name)
    : m_name(std::move(name))
    , m_sampler(settings.Sampler)
{   
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(!settings.SizeIsExplicitlySpecified, "Unable to specify 2d texture size when loading from file");

    TextureLoadResult loaded = TextureLoad::FromFile(path.c_str(), TextureLoadSettings().SetOutFormat(settings.Format));
    Initialize(loaded.GetData(), { loaded.GetWidth(), loaded.GetHeight() }, settings);
}

OpenGLTexture2D::OpenGLTexture2D(const void* data, const Texture2DSettings& settings, std::string name)
    : m_name(std::move(name))
    , m_sampler(settings.Sampler)
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(settings.SizeIsExplicitlySpecified, "Texture dimensions are not specified");
    Initialize(data, settings.DesiredSize, settings);
}

void OpenGLTexture2D::Initialize(const void* data, const glm::u32vec2& size, const Texture2DSettings& settings)
{
    PN_PROFILE_FUNCTION();

    glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);

    m_size = size;
    m_format = ToGlFormat(settings.Format);
    m_numChannels = GetNumChannels(settings.Format);
    m_numLevels = settings.NumLevels;
    if (m_numLevels == 0) {
        m_numLevels = GetNumTextureLevels(size);
    }

    int numLevels = settings.GenerateMipmaps ? GetNumTextureLevels(m_size) : 1;
    glTextureStorage2D(m_handle, m_numLevels, ToGlInternalFormat(settings.Format), m_size.x, m_size.y);

    if (data) {
        glTextureSubImage2D(m_handle, 0, 0, 0, m_size.x, m_size.y, m_format, GL_UNSIGNED_BYTE, data);
        if (settings.GenerateMipmaps) {
            glGenerateTextureMipmap(m_handle);
        }
    }
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_handle);
}

void OpenGLTexture2D::SetData(const void* data, const glm::u32vec2& size, const glm::u32vec2& offset)
{
    PN_PROFILE_FUNCTION();

    SetLevelData(data, 0, offset, size);
    glGenerateTextureMipmap(m_handle);
}

void OpenGLTexture2D::SetLevelData(const void* data, uint32_t level, const glm::u32vec2& size, const glm::u32vec2& offset)
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(data, "Data cannot be nullptr");
    
    glm::u32vec2 sz = size;
    if (size.x == 0 && size.y == 0) {
        sz = GetLevelDimensions(level) - offset;
    }

    glTextureSubImage2D(m_handle, level, offset.x, offset.y, sz.x, sz.y, m_format, GL_UNSIGNED_BYTE, data);
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

glm::u32vec2 OpenGLTexture2D::GetLevelDimensions(uint32_t level) const
{
    glm::u32vec2 result = m_size;
    for (uint32_t i = 0; i < level; i++) {
        result.x = std::max(1u, result.x / 2);
        result.y = std::max(1u, result.y / 2);

        if (result.x == 1 && result.y == 1) {
            break;
        }
    }

    return result;
}


}
