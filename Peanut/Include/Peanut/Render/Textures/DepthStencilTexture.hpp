#pragma once

#include "Texture.hpp"
#include <Peanut/Render/Textures/TextureSampler.hpp>

#include <string>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>

namespace pn {

    enum class DepthFormat : uint32_t
    {
        Float24 = 0,
        Float32,
    };

    enum class StencilFormat : uint32_t
    {
        Uint8 = 0,
    };

    struct DepthStencilTextureSettings
    {
    public:
        DepthStencilTextureSettings& UseDepth(bool use) { UsingDepth = use; return *this; }
        DepthStencilTextureSettings& UseStencil(bool use) { UsingStencil = use; return *this; }

        DepthStencilTextureSettings& SetDepthFormat(DepthFormat format) { DepthFmt = format; return *this; }
        DepthStencilTextureSettings& SetStencilFormat(StencilFormat format) { StencilFmt = format; return *this; }

        DepthStencilTextureSettings& SetSize(uint32_t width, uint32_t height) { Size.x = width; Size.y = height; return *this; }

    public:
        bool UsingDepth = true;
        bool UsingStencil = true;
        DepthFormat DepthFmt = DepthFormat::Float32;
        StencilFormat StencilFmt = StencilFormat::Uint8;
        glm::u32vec2 Size = { 0, 0 };
    };

    class DepthStencilTexture : virtual public Texture
    {
    public:
        ~DepthStencilTexture() override = default;

        virtual const glm::u32vec2& GetSize() const = 0;

        static std::shared_ptr<DepthStencilTexture> Create(const DepthStencilTextureSettings& settings, const std::string& name = "");
    };

}
