#ifndef __OPENGLTEXTURE_H__
#define __OPENGLTEXTURE_H__

#include <Peanut/Render/Textures/Texture2D.hpp>

namespace pn {

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const std::string& path, std::string name = "");
    OpenGLTexture2D(const TextureData& data, std::string name = "");
    ~OpenGLTexture2D() override;

    const std::string& GetName() const override { return m_name; }

    void BindToSlot(uint32_t slot = 0) override;
    void Unbind() override;

private:
    std::string m_name;
    uint32_t m_descriptor;
    uint32_t m_slot = 0;
};

}

#endif
