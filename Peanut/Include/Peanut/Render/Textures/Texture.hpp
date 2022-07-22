#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>
#include <memory>
#include <vector>

namespace pn {

struct TextureData
{
    std::vector<uint8_t> bytes;
};

class Texture
{
public:
    Texture() = default;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    virtual ~Texture() = default;

    virtual const std::string& GetName() const = 0;

    virtual void BindToSlot(uint32_t slot = 0) = 0;
    virtual void Unbind() = 0;
};

}

#endif
