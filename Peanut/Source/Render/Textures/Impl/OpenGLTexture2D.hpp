#pragma once

#include <Peanut/Render/Textures/Texture2D.hpp>
#include "OpenGLTexture.hpp"

namespace pn {

class OpenGLTexture2D : public Texture2D, public OpenGLTexture
{
public:
    OpenGLTexture2D(const std::string& path, const Texture2DSettings& settings, std::string name = "");
    OpenGLTexture2D(const void* data, const Texture2DSettings& settings, std::string name = "");
    ~OpenGLTexture2D() override;

    bool operator==(const Texture& other) const override { return OpenGLTexture::operator==(other); }

    void SetSampler(const std::shared_ptr<TextureSampler>& sampler) { m_sampler = sampler; }
    std::shared_ptr<TextureSampler> GetSampler() const override { return m_sampler; }

    uint32_t GetNumMipmaps() const override { return m_numLevels; }

    const std::string& GetName() const override { return m_name; }
    const glm::u32vec2& GetSize() const override { return m_size; }

    void SetData(const void* data, const glm::u32vec2& size = { 0, 0 }, const glm::u32vec2& offset = { 0, 0 }) override;
    void SetLevelData(const void* data, uint32_t level, const glm::u32vec2& size = {0, 0}, const glm::u32vec2& offset = { 0, 0 }) override;

private:
    std::string m_name;
    glm::u32vec2 m_size;
    int m_format;
    uint32_t m_numChannels;
    uint32_t m_numLevels;
    std::shared_ptr<TextureSampler> m_sampler;

private:
    void Initialize(const void* data, const glm::u32vec2& size, const Texture2DSettings& settings);
};

}
