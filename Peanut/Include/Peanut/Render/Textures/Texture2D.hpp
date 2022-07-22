#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include "Texture.hpp"

#include <string>
#include <memory>
#include <vector>

namespace pn {

class Texture2D : public Texture
{
public:
    virtual ~Texture2D() = default;

    virtual size_t GetX() const = 0;
    virtual size_t GetY() const = 0;

    virtual void SetWrapping(TextureWrap x, TextureWrap y) = 0;
    virtual void SetFiltering(TextureFilter min, TextureFilter mag) = 0;
    virtual void SetFiltering(TextureMipmapFilter min, TextureMipmapFilter mag) = 0;

    static std::shared_ptr<Texture2D> Create(const std::string& path, const std::string& name = "");
    static std::shared_ptr<Texture2D> Create(const TextureData& data, const std::string& name = "");
};

}

#endif
