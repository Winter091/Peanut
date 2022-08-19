#pragma once

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

    virtual const std::string& GetName() const = 0;

    virtual void BindToSlot(uint32_t slot = 0) = 0;
};

}
