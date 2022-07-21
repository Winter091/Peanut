#include "OpenGLTexture2D.hpp"

#include <Peanut/Core/Assert.hpp>
#include <stb/stb_image.h>
#include <glad/glad.h>

#include <cstdio>

namespace pn {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path, std::string name)
    : m_name(std::move(name))
{   
    FILE* f = fopen(path.c_str(), "r");
    PN_CORE_ASSERT(f, "Unable to open texture file {}", path);
    
    int x, y, channels;
    unsigned char* data = stbi_load_from_file(f, &x, &y, &channels, 4);
    if (channels < 3) {
        PN_CORE_WARN("Texture {}: number of channels is < 3", path);
    }

    glGenTextures(1, &m_descriptor);
    glBindTexture(GL_TEXTURE_2D, m_descriptor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

OpenGLTexture2D::OpenGLTexture2D(const TextureData& data, std::string name)
    : m_name(std::move(name))
{
    (void)data, (void)name;
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_descriptor);
}

void OpenGLTexture2D::BindToSlot(uint32_t slot)
{
    m_slot = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_descriptor);
}

void OpenGLTexture2D::Unbind()
{
    glActiveTexture(GL_TEXTURE0 + m_slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
