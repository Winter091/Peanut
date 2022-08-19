#pragma once

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Textures/TextureSampler.hpp>

#include <glad/glad.h>

namespace pn {

    inline uint32_t GetGlMinFilter(TextureFilter minFilter, TextureMipFilter mipFilter)
    {
        switch (mipFilter) {
            case TextureMipFilter::Nearest: 
            {
                switch (minFilter) {
                    case TextureFilter::Nearest:        return GL_NEAREST_MIPMAP_NEAREST;
                    case TextureFilter::Linear: 
                    case TextureFilter::Anisotropic:    return GL_LINEAR_MIPMAP_NEAREST;
                    default: break;
                }
            }
            case TextureMipFilter::Linear:
            {
                switch (minFilter) {
                    case TextureFilter::Nearest:        return GL_NEAREST_MIPMAP_LINEAR;
                    case TextureFilter::Linear:
                    case TextureFilter::Anisotropic:    return GL_LINEAR_MIPMAP_LINEAR;
                    default: break;
                }
            }
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown combination of TextureFilter and TextureMipFilter enum values");
        return GL_LINEAR_MIPMAP_LINEAR;
    }

    inline uint32_t GetGlMagFilter(TextureFilter magFilter)
    {
        switch (magFilter) {
            case TextureFilter::Nearest:        return GL_NEAREST;
            case TextureFilter::Linear:
            case TextureFilter::Anisotropic:    return GL_LINEAR;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureFilter enum value");
        return GL_LINEAR_MIPMAP_LINEAR;
    }

    inline uint32_t GetGLWrap(TextureWrap wrap)
    {
        switch (wrap) {
        case TextureWrap::Repeat:           return GL_REPEAT;
        case TextureWrap::MirroredRepeat:   return GL_MIRRORED_REPEAT;
        case TextureWrap::Clamp:            return GL_CLAMP_TO_EDGE;
        case TextureWrap::Border:           return GL_CLAMP_TO_BORDER;
        default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureWrap enum value");
        return GL_REPEAT;
    }

}
