#ifndef __OPENGLTEXTURE_H__
#define __OPENGLTEXTURE_H__

#include <Peanut/Render/Textures/Texture.hpp>

namespace pn {

class OpenGLTexture : public Texture
{
public:
    OpenGLTexture(const std::string& path, std::string name = "");
    OpenGLTexture(const TextureData& data, std::string name = "");
    ~OpenGLTexture() override;

    const std::string& GetName() const override { return m_name; }

    void Bind() override;
    void Unbind() override;

private:
    std::string m_name;
};

}

#endif
