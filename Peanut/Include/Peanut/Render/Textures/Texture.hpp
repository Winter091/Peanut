#pragma once

#include <Peanut/Render/Textures/TextureSampler.hpp>
#include <Peanut/Core/Assert.hpp>

#include <string>
#include <memory>

namespace pn {

enum class TextureFormat
{
    RGBA
};

inline uint32_t GetTextureFormatNumChannels(TextureFormat format)
{
    switch (format) {
        case TextureFormat::RGBA: return 4;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
    return 4;
}

class Texture
{
public:
    Texture() = default;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    virtual ~Texture() = default;

    virtual bool operator==(const Texture& other) const = 0;

    virtual void SetSampler(const std::shared_ptr<TextureSampler>& sampler) = 0;
    virtual std::shared_ptr<TextureSampler> GetSampler() const = 0;

    virtual uint32_t GetNumMipmaps() const = 0;

    virtual const std::string& GetName() const = 0;
};

}
