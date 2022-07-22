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

    size_t GetX() const override { return m_x; }
    size_t GetY() const override { return m_y; } 

    void SetWrapping(TextureWrap x, TextureWrap y) override;
    void SetFiltering(TextureFilter min, TextureFilter mag) override;
    void SetFiltering(TextureMipmapFilter min, TextureMipmapFilter mag) override;

private:
    std::string m_name;
    uint32_t m_descriptor;
    uint32_t m_slot = 0;
    size_t m_x, m_y;

private:
    size_t GetFileSize(FILE* f);
    void InitializeFromData(const TextureData& data);
    int ToGlWrap(TextureWrap wrap) const;
    int ToGlFilter(TextureFilter filter) const;
    int ToGlFilter(TextureMipmapFilter filter) const;
};

}

#endif
