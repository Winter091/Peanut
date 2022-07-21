#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include <string>
#include <memory>
#include <vector>

namespace pn {

struct TextureData
{
    std::vector<uint8_t> bytes;
};

class Texture2D
{
public:
    Texture2D() = default;
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    virtual ~Texture2D() = default;

    virtual const std::string& GetName() const = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    static std::shared_ptr<Texture2D> Create(const std::string& path, const std::string& name = "");
    static std::shared_ptr<Texture2D> Create(const TextureData& data, const std::string& name = "");
};

}

#endif
