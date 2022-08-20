#pragma once

#include <Peanut/Render/Textures/Texture2D.hpp>

namespace pn {

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const std::string& path, const Texture2DSettings& settings, std::string name = "");
    OpenGLTexture2D(const TextureData& data, const Texture2DSettings& settings, std::string name = "");
    ~OpenGLTexture2D() override;

    bool operator==(const Texture2D& other) const override { return m_descriptor == other.GetDescriptor(); }

    uint32_t GetDescriptor() const override { return m_descriptor; }

    void SetSampler(const std::shared_ptr<TextureSampler>& sampler) { m_sampler = sampler; }
    std::shared_ptr<TextureSampler> GetSampler() const override { return m_sampler; }

    const std::string& GetName() const override { return m_name; }

    void BindToSlot(uint32_t slot = 0) override;

    const glm::u32vec2& GetSize() const override { return m_size; }

    void SetData(const TextureData& data, const glm::u32vec2& offset = {0, 0}, const glm::u32vec2& size = {0, 0}) override;

private:
    std::string m_name;
    uint32_t m_descriptor;
    uint32_t m_slot = 0;
    glm::u32vec2 m_size;
    int m_format;
    uint32_t m_numChannels;
    bool m_useMipmaps;
    std::shared_ptr<TextureSampler> m_sampler;

private:
    void Initialize(const void* data, const glm::u32vec2& size, const Texture2DSettings& settings);
    int ToGlInternalFormat(TextureFormat format) const;
    int ToGlFormat(TextureFormat format) const;
    uint32_t GetNumChannels(TextureFormat format) const;
    uint32_t GetNumTextureLevels(const glm::u32vec2& textureSize) const;
};

}
