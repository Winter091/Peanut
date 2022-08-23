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

OpenGLTexture2D::OpenGLTexture2D(const TextureData& data, const Texture2DSettings& settings, std::string name)
    : m_name(std::move(name))
    , m_sampler(settings.Sampler)
{
    PN_PROFILE_FUNCTION();

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
    PN_PROFILE_FUNCTION();

    glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);

    m_size = size;

    m_format = ToGlFormat(settings.Format);
    m_numChannels = GetNumChannels(settings.Format);

    m_useMipmaps = settings.UseMipmaps;

    int numTextuteLevels = m_useMipmaps ? GetNumTextureLevels(m_size) : 1;
    glTextureStorage2D(m_handle, numTextuteLevels, ToGlInternalFormat(settings.Format), m_size.x, m_size.y);

    if (data) {
        glTextureSubImage2D(m_handle, 0, 0, 0, m_size.x, m_size.y, m_format, GL_UNSIGNED_BYTE, data);
        if (m_useMipmaps) {
            glGenerateTextureMipmap(m_handle);
        }
    }

    PN_CORE_DEBUG("Texture after init has handle = {}", m_handle);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_handle);
}

void OpenGLTexture2D::SetData(const TextureData& data, const glm::u32vec2& offset, const glm::u32vec2& size)
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(!data.empty(), "No data to set");
    PN_CORE_ASSERT(data.size() <= m_size.x * m_size.y * m_numChannels, "Provided data size is larger than the texture can possibly hold");
    PN_CORE_ASSERT(offset.x + size.x < m_size.x && offset.y + size.y < m_size.y, "Subrectangle defined by offset and size is out of bounds");
    
    glm::u32vec2 sz = size;
    if (size.x == 0 && size.y == 0) {
        sz = m_size - offset;
    }

    PN_CORE_ASSERT(data.size() >= sz.x * sz.y * m_numChannels, "Provided data size is not enough for specified subrectangle");

    glTextureSubImage2D(m_handle, 0, offset.x, offset.y, sz.x, sz.y, m_format, GL_UNSIGNED_BYTE, &data[0]);
    if (m_useMipmaps) {
        glGenerateTextureMipmap(m_handle);
    }
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
