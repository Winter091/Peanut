#pragma once

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Textures/Texture.hpp>
#include <Peanut/Render/Textures/TextureSampler.hpp>

#include <d3d11.h>

namespace pn {

    inline D3D11_FILTER GetDx11Filter(TextureFilter filter)
    {
        switch (filter) {
            case TextureFilter::MinPointMagPointMipPoint: return D3D11_FILTER_MIN_MAG_MIP_POINT;
            case TextureFilter::MinPointMagPointMipLinear: return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
            case TextureFilter::MinPointMagLinearMipPoint: return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
            case TextureFilter::MinPointMagLinearMipLinear: return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
            case TextureFilter::MinLinearMagPointMipPoint: return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
            case TextureFilter::MinLinearMagPointMipLinear: return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
            case TextureFilter::MinLinearMagLinearMipPoint: return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
            case TextureFilter::MinLinearMagLinearMipLinear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            case TextureFilter::MinAnisotropicMagAnisotropicMipAnisotropic: return D3D11_FILTER_ANISOTROPIC;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureWrap enum value");
        return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    }

    inline D3D11_TEXTURE_ADDRESS_MODE  GetDx11AddressMode(TextureWrap wrap)
    {
        switch (wrap) {
            case TextureWrap::Repeat: return D3D11_TEXTURE_ADDRESS_WRAP;
            case TextureWrap::MirroredRepeat: return D3D11_TEXTURE_ADDRESS_MIRROR;
            case TextureWrap::Clamp: return D3D11_TEXTURE_ADDRESS_CLAMP;
            case TextureWrap::Border: return D3D11_TEXTURE_ADDRESS_BORDER;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureWrap enum value");
        return D3D11_TEXTURE_ADDRESS_WRAP;
    }

    inline D3D11_COMPARISON_FUNC GetDx11ComparisonFunc(TextureComparisonFunc func)
    {
        switch (func) {
            case TextureComparisonFunc::NotRequired: return D3D11_COMPARISON_ALWAYS;
            case TextureComparisonFunc::Never: return D3D11_COMPARISON_NEVER;
            case TextureComparisonFunc::Less: return D3D11_COMPARISON_LESS;
            case TextureComparisonFunc::Equal: return D3D11_COMPARISON_EQUAL;
            case TextureComparisonFunc::LessEqual: return D3D11_COMPARISON_LESS_EQUAL;
            case TextureComparisonFunc::Greater: return D3D11_COMPARISON_GREATER;
            case TextureComparisonFunc::NotEqual: return D3D11_COMPARISON_NOT_EQUAL;
            case TextureComparisonFunc::GreaterEqual: return D3D11_COMPARISON_GREATER_EQUAL;
            case TextureComparisonFunc::Always: return D3D11_COMPARISON_ALWAYS;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureComparisonFunc enum value");
        return D3D11_COMPARISON_ALWAYS;
    }

    inline DXGI_FORMAT GetDx11TextureFormat(TextureFormat format)
    {
        switch (format) {
            case TextureFormat::RGBA:   return DXGI_FORMAT_R8G8B8A8_UNORM;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    }

}
