#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include "Texture.hpp"

#include <string>
#include <memory>
#include <vector>

namespace pn {

struct Texture2DSettings
{
    TextureFormat Format = TextureFormat::RGBA;
    TextureWrap WrapX = TextureWrap::Repeat; 
    TextureWrap WrapY = TextureWrap::Repeat;
    TextureFilter FilterMin = TextureFilter::Linear;
    TextureFilter FilterMag = TextureFilter::Linear;
    TextureMipmapFilter MipmapFilterMin = TextureMipmapFilter::LinearMipmapLinear; 
    bool UseMipmaps = true;

    Texture2DSettings& UseFormat(TextureFormat format)
    {
        Format = format;
        return *this;
    }

    Texture2DSettings& UseWrapping(TextureWrap x, TextureWrap y)
    {
        WrapX = x;
        WrapY = y;
        return *this;
    }

    Texture2DSettings& UseFiltering(TextureFilter min, TextureFilter mag)
    {
        FilterMin = min;
        FilterMag = mag;
        return *this;
    }

    Texture2DSettings& UseMipmapFiltering(TextureMipmapFilter min, TextureFilter mag)
    {
        UseMipmaps = true;
        MipmapFilterMin = min;
        FilterMag = mag;
        return *this;
    }

    Texture2DSettings& UseMipmapping(bool use)
    {
        UseMipmaps = use;
        return *this;
    }
};

class Texture2D : public Texture
{
public:
    virtual ~Texture2D() = default;

    virtual size_t GetX() const = 0;
    virtual size_t GetY() const = 0;

    virtual void SetWrapping(TextureWrap x, TextureWrap y) = 0;
    virtual void SetFiltering(TextureFilter min, TextureFilter mag) = 0;
    virtual void SetFiltering(TextureMipmapFilter min, TextureFilter mag) = 0;

    static std::shared_ptr<Texture2D> Create(const std::string& path, const Texture2DSettings& settings, const std::string& name = "");
    static std::shared_ptr<Texture2D> Create(const TextureData& data, const Texture2DSettings& settings, const std::string& name = "");
};

}

#endif
