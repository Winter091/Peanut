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

    static std::shared_ptr<Texture2D> Create(const std::string& path, const std::string& name = "");
    static std::shared_ptr<Texture2D> Create(const TextureData& data, const std::string& name = "");
};

}

#endif
