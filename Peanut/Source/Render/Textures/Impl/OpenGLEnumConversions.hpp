#pragma once

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Textures/TextureSampler.hpp>

#include <glad/glad.h>

namespace pn {

    inline uint32_t GetGlMinFilter(TextureFilter filter)
    {
        switch (filter) {
            case TextureFilter::MinPointMagPointMipPoint: 
            case TextureFilter::MinPointMagLinearMipPoint: return GL_NEAREST_MIPMAP_NEAREST;

            case TextureFilter::MinPointMagPointMipLinear: 
            case TextureFilter::MinPointMagLinearMipLinear: return GL_NEAREST_MIPMAP_LINEAR;

            case TextureFilter::MinLinearMagPointMipPoint:
            case TextureFilter::MinLinearMagLinearMipPoint: return GL_LINEAR_MIPMAP_NEAREST;

            case TextureFilter::MinLinearMagPointMipLinear:
            case TextureFilter::MinLinearMagLinearMipLinear: 
            case TextureFilter::MinAnisotropicMagAnisotropicMipAnisotropic: return GL_LINEAR_MIPMAP_LINEAR;
        }

        PN_CORE_ASSERT(false, "Unknown TextureFilter enum value");
        return GL_LINEAR_MIPMAP_LINEAR;
    }

    inline uint32_t GetGlMagFilter(TextureFilter filter)
    {
        switch (filter) {
            case TextureFilter::MinPointMagPointMipPoint: 
            case TextureFilter::MinLinearMagPointMipPoint:
            case TextureFilter::MinPointMagPointMipLinear: 
            case TextureFilter::MinLinearMagPointMipLinear: return GL_NEAREST;

            case TextureFilter::MinPointMagLinearMipPoint:
            case TextureFilter::MinLinearMagLinearMipPoint: 
            case TextureFilter::MinPointMagLinearMipLinear:
            case TextureFilter::MinLinearMagLinearMipLinear: 
            case TextureFilter::MinAnisotropicMagAnisotropicMipAnisotropic: return GL_LINEAR;
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
