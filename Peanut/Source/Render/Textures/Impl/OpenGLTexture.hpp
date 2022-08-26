#pragma once

#include <Peanut/Render/Textures/Texture.hpp>

namespace pn {

    class OpenGLTexture : virtual public Texture
    {
    public:
        ~OpenGLTexture() override = default;

        bool operator==(const Texture& other) const override { return m_handle == dynamic_cast<const OpenGLTexture&>(other).GetOpenGLHandle(); }

        uint32_t GetOpenGLHandle() const { return m_handle; }

    protected:
        uint32_t m_handle = 0;
    };

}
