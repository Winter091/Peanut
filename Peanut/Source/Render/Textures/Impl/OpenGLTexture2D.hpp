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

    void Bind() override;
    void Unbind() override;

private:
    std::string m_name;
};

}

#endif
