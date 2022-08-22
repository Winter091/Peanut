#pragma once

#include <Peanut/Render/Textures/TextureSampler.hpp>

#include <string>
#include <memory>
#include <vector>

namespace pn {

enum class TextureFormat
{
    RGB,
    RGBA
};

using TextureData = std::vector<uint8_t>;

class Texture
{
public:
    Texture() = default;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    virtual ~Texture() = default;

    virtual bool operator==(const Texture& other) const = 0;

    virtual void SetSampler(const std::shared_ptr<TextureSampler>& sampler) = 0;
    virtual  std::shared_ptr<TextureSampler> GetSampler() const = 0;

    virtual const std::string& GetName() const = 0;
};

}
