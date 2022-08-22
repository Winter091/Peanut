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

    Texture2DSettings& UseMipmapping(bool use)
    {
        UseMipmaps = use;
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
    bool UseMipmaps = true;
    std::shared_ptr<TextureSampler> Sampler = nullptr;
};

class Texture2D : virtual public Texture
{
public:
    ~Texture2D() override = default;

    virtual const glm::u32vec2& GetSize() const = 0;

    virtual void SetData(const TextureData& data, const glm::u32vec2& offset = {0, 0}, const glm::u32vec2& size = {0, 0}) = 0;

    static std::shared_ptr<Texture2D> Create(const std::string& path, const Texture2DSettings& settings, const std::string& name = "");
    static std::shared_ptr<Texture2D> Create(const TextureData& data, const Texture2DSettings& settings, const std::string& name = "");
    static std::shared_ptr<Texture2D> Create(const glm::vec2& size, const TextureData& initialData, const Texture2DSettings& settings, const std::string& name = "");
};

}
