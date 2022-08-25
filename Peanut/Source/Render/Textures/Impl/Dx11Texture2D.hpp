#pragma once

#include <Peanut/Render/Textures/Texture2D.hpp>
#include "Dx11Texture.hpp"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace pn {

    class Dx11Texture2D : public Texture2D, public Dx11Texture
    {
    public:
        Dx11Texture2D(const std::string& path, const Texture2DSettings& settings, std::string name = "");
        Dx11Texture2D(const void* data, const Texture2DSettings& settings, std::string name = "");
        ~Dx11Texture2D() override;

        bool operator==(const Texture& other) const override { return Dx11Texture::operator==(other); }

        void SetSampler(const std::shared_ptr<TextureSampler>& sampler) { m_sampler = sampler; }
        std::shared_ptr<TextureSampler> GetSampler() const override { return m_sampler; }

        uint32_t GetNumMipmaps() const override { return m_numLevels; }

        const std::string& GetName() const override { return m_name; }
        const glm::u32vec2& GetSize() const override { return m_size; }

        void SetData(const void* data, const glm::u32vec2& size = { 0, 0 }, const glm::u32vec2& offset = { 0, 0 }) override;
        void SetLevelData(const void* data, uint32_t level, const glm::u32vec2& size = { 0, 0 }, const glm::u32vec2& offset = { 0, 0 }) override;

    private:
        ID3D11Texture2D* m_handle = nullptr;
        ID3D11ShaderResourceView* m_view = nullptr;
        std::string m_name;
        glm::u32vec2 m_size;
        std::shared_ptr<TextureSampler> m_sampler;
        std::uint32_t m_numLevels;
        std::uint32_t m_numChannels;

    private:
        void Initialize(const void* data, const glm::u32vec2& size, const Texture2DSettings& settings);
    };

}
