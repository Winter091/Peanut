#include "Dx11DepthStencilTexture.hpp"

#include "Dx11EnumConversions.hpp"
#include <Window/RenderContextImpl/Dx11RenderContext.hpp>

#include <d3d11.h>

namespace pn {

	Dx11DepthStencilTexture::Dx11DepthStencilTexture(const DepthStencilTextureSettings& settings, std::string name)
	{
        auto* device = Dx11RenderContext::GetCurrentContext().GetDevice();
        DXGI_FORMAT textureFormat = GetDx11DepthStencilTextureFormat(settings.DepthFmt, settings.UsingDepth, settings.StencilFmt, settings.UsingStencil);
        
        {
            D3D11_TEXTURE2D_DESC textureDesc;
            textureDesc.Width = settings.Size.x;
            textureDesc.Height = settings.Size.y;
            textureDesc.MipLevels = 1;
            textureDesc.ArraySize = 1;
            textureDesc.Format = textureFormat;
            textureDesc.SampleDesc.Count = 1;
            textureDesc.SampleDesc.Quality = 0;
            textureDesc.Usage = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
            textureDesc.CPUAccessFlags = 0;
            textureDesc.MiscFlags = 0;

            HRESULT res = device->CreateTexture2D(&textureDesc, nullptr, &m_handle);
            PN_CORE_ASSERT(res == S_OK, "Unable to create depth stencil texture");
        }

        {
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
            depthStencilViewDesc.Format = GetDx11DepthStencilViewFormat(textureFormat);
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Flags = 0;
            depthStencilViewDesc.Texture2D.MipSlice = 0;

            HRESULT res = device->CreateDepthStencilView(m_handle, &depthStencilViewDesc, &m_depthStencilView);
            PN_CORE_ASSERT(res == S_OK, "Unable to create depth stencil view");
        }

        {
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
            shaderResourceViewDesc.Format = GetDx11ShaderResourceFormat(textureFormat);
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            HRESULT res = device->CreateShaderResourceView(m_handle, &shaderResourceViewDesc, &m_shaderResourceView);
            PN_CORE_ASSERT(res == S_OK, "Unable to create shader resource view for depth stencil texture");
        }

        Dx11Texture::SetNativeTexturePtr(m_handle);
        Dx11Texture::SetNativeViewPtr(m_shaderResourceView);
	}

	Dx11DepthStencilTexture::~Dx11DepthStencilTexture()
	{
        m_depthStencilView->Release();
        m_shaderResourceView->Release();
        m_handle->Release();
	}

}
