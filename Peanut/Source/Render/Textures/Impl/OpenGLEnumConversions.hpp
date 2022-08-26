#pragma once

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Textures/Texture.hpp>
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
            case TextureWrap::Repeat: return GL_REPEAT;
            case TextureWrap::MirroredRepeat:return GL_MIRRORED_REPEAT;
            case TextureWrap::Clamp: return GL_CLAMP_TO_EDGE;
            case TextureWrap::Border: return GL_CLAMP_TO_BORDER;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureWrap enum value");
        return GL_REPEAT;
    }

    inline uint32_t GetGlComparisonFunc(TextureComparisonFunc func)
    {
        switch (func) {
            case TextureComparisonFunc::Never : return GL_NEVER;
            case TextureComparisonFunc::Less: return GL_LESS;
            case TextureComparisonFunc::Equal: return GL_EQUAL;
            case TextureComparisonFunc::LessEqual: return GL_LEQUAL;
            case TextureComparisonFunc::Greater: return GL_GREATER;
            case TextureComparisonFunc::NotEqual: return GL_NOTEQUAL;
            case TextureComparisonFunc::GreaterEqual: return GL_GEQUAL;
            case TextureComparisonFunc::Always: return GL_ALWAYS;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureComparisonFunc enum value");
        return GL_ALWAYS;
    }

    inline uint32_t GetGlTextureFormat(TextureFormat format)
    {
        switch (format) {
            case TextureFormat::RGBA:   return GL_RGBA;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
        return GL_RGBA;
    }

    inline uint32_t GetGlInternalTextureFormat(TextureFormat format)
    {
        switch (format) {
            case TextureFormat::RGBA:   return GL_RGBA8;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
        return GL_RGBA8;
    }

}
