#include "Dx11Texture2D.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Textures/TextureLoad.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include "Dx11EnumConversions.hpp"

#include <d3d11.h>

namespace pn {

	Dx11Texture2D::Dx11Texture2D(const std::string& path, const Texture2DSettings& settings, std::string name)
		: m_name(std::move(name))
		, m_sampler(settings.Sampler)
	{
		PN_PROFILE_FUNCTION();

		PN_CORE_ASSERT(!settings.SizeIsExplicitlySpecified, "Unable to specify 2d texture size when loading from file");

		TextureLoadResult loaded = TextureLoad::FromFile(path.c_str(), TextureLoadSettings().SetOutFormat(settings.Format));
		Initialize(loaded.GetData(), { loaded.GetWidth(), loaded.GetHeight() }, settings);
	}

	Dx11Texture2D::Dx11Texture2D(const void* data, const Texture2DSettings& settings, std::string name)
		: m_name(std::move(name))
		, m_sampler(settings.Sampler)
	{
		PN_CORE_ASSERT(settings.SizeIsExplicitlySpecified, "Texture dimensions are not specified");
		Initialize(data, settings.DesiredSize, settings);
	}

	Dx11Texture2D::~Dx11Texture2D()
	{
		m_view->Release();
		m_handle->Release();
	}

	void Dx11Texture2D::Initialize(const void* data, const glm::u32vec2& size, const Texture2DSettings& settings)
	{
		m_size = size;
		m_numLevels = settings.NumLevels;
		if (m_numLevels == 0) {
			m_numLevels = Texture2D::GetMaxAmountOfMips(m_size);
		}

		m_numChannels = GetTextureFormatNumChannels(settings.Format);
		
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = m_size.x;
		desc.Height = m_size.y;
		desc.MipLevels = m_numLevels;
		desc.ArraySize = 1;
		desc.Format = GetDx11TextureFormat(settings.Format);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		auto* device = Dx11GLFWRenderContext::GetCurrentContext().GetDevice();
		HRESULT res = device->CreateTexture2D(&desc, nullptr, &m_handle);
		PN_CORE_ASSERT(res == S_OK, "Unable to create 2d texture");

		res = device->CreateShaderResourceView(m_handle, nullptr, &m_view);
		PN_CORE_ASSERT(res == S_OK, "Unable to create 2d texture shader resource view");

		if (data) {
			SetLevelData(data, 0);
			if (settings.GenerateMipmaps) {
				auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
				deviceContext->GenerateMips(m_view);
			}
		}

		Dx11Texture::SetNativeTexturePtr(m_handle);
		Dx11Texture::SetNativeViewPtr(m_view);
	}

	void Dx11Texture2D::SetData(const void* data, const glm::u32vec2& size, const glm::u32vec2& offset)
	{
		SetLevelData(data, 0, size, offset);

		auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
		deviceContext->GenerateMips(m_view);
	}

	void Dx11Texture2D::SetLevelData(const void* data, uint32_t level, const glm::u32vec2& size, const glm::u32vec2& offset)
	{
		PN_CORE_ASSERT(data, "Cannot set nullptr data");

		glm::u32vec2 sz = size;
		if (size.x == 0 && size.y == 0) {
			sz = Texture2D::GetMipLevelDimensions(m_size, level) - offset;
		}
		
		D3D11_BOX region;
		region.left = offset.x;
		region.right = offset.x + sz.x;
		region.top = offset.y;
		region.bottom = offset.y + sz.y;
		region.front = 0;
		region.back = 1;

		auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
		deviceContext->UpdateSubresource(m_handle, level, &region, data, sz.x * m_numChannels, 0);
	}

}


