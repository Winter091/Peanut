#pragma once

#include <Peanut/Render/Textures/DepthStencilTexture.hpp>
#include "Dx11Texture.hpp"

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;

namespace pn {

    class Dx11DepthStencilTexture : public DepthStencilTexture, public Dx11Texture
    {
    public:
        Dx11DepthStencilTexture(const DepthStencilTextureSettings& settings, std::string name = "");
        ~Dx11DepthStencilTexture() override;

        bool operator==(const Texture& other) const override { return Dx11Texture::operator==(other); }

        void SetSampler(const std::shared_ptr<TextureSampler>& sampler) { m_sampler = sampler; }
        std::shared_ptr<TextureSampler> GetSampler() const override { return m_sampler; }

        uint32_t GetNumMipmaps() const override { return 1u; }

        ID3D11DepthStencilView* GetNativeDepthStencilView() const { return m_depthStencilView; }

        const std::string& GetName() const override { return m_name; }
        const glm::u32vec2& GetSize() const override { return m_size; }

    private:
        ID3D11Texture2D* m_handle = nullptr;
        ID3D11DepthStencilView* m_depthStencilView = nullptr;
        ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
        std::string m_name;
        glm::u32vec2 m_size;
        std::shared_ptr<TextureSampler> m_sampler;
    };

}
