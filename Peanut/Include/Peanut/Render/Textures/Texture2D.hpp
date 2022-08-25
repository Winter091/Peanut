#pragma once

#include "Texture.hpp"
#include <Peanut/Render/Textures/TextureSampler.hpp>

#include <string>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>

namespace pn {

struct Texture2DSettings
{
    Texture2DSettings& UseFormat(TextureFormat format) 
    {
        Format = format;
        return *this;
    }

    Texture2DSettings& SetNumLevels(uint32_t numLevels)
    {
        NumLevels = numLevels;
        return *this;
    }

    Texture2DSettings& DoGenerateMipmaps(bool generate)
    {
        GenerateMipmaps = generate;
        return *this;
    }

    Texture2DSettings& SetSize(const glm::u32vec2& size)
    {
        DesiredSize = size;
        SizeIsExplicitlySpecified = true;
        return *this;
    }

    Texture2DSettings SetSampler(const std::shared_ptr<TextureSampler>& sampler)
    {
        Sampler = sampler;
        return *this;
    }

    glm::u32vec2 DesiredSize = {0, 0};
    bool SizeIsExplicitlySpecified = false;
    TextureFormat Format = TextureFormat::RGBA;
    uint32_t NumLevels = 0;
    bool GenerateMipmaps = false;
    std::shared_ptr<TextureSampler> Sampler = nullptr;
};

class Texture2D : virtual public Texture
{
public:
    ~Texture2D() override = default;

    static glm::u32vec2 GetMipLevelDimensions(const glm::u32vec2& mainDimensions, uint32_t level);
    static uint32_t GetMaxAmountOfMips(const glm::u32vec2& mainDimensions);

    virtual const glm::u32vec2& GetSize() const = 0;

    virtual void SetData(const void* data, const glm::u32vec2& size = { 0, 0 }, const glm::u32vec2& offset = { 0, 0 }) = 0;
    virtual void SetLevelData(const void* data, uint32_t level, const glm::u32vec2& size = { 0, 0 }, const glm::u32vec2& offset = { 0, 0 }) = 0;

    static std::shared_ptr<Texture2D> Create(const std::string& path, const Texture2DSettings& settings, const std::string& name = "");
    static std::shared_ptr<Texture2D> Create(const void* data, const Texture2DSettings& settings, const std::string& name = "");
};

}
