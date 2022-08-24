#include "Dx11Texture2D.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Textures/TextureLoad.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>

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
			m_numLevels = GetNumTextureLevels(size);
		}
		
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = m_size.x;
		desc.Height = m_size.y;
		desc.MipLevels = m_numLevels;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

		glm::u32vec2 levelDimensions = GetLevelDimensions(level);

		glm::u32vec2 sz = size;
		if (size.x == 0 && size.y == 0) {
			sz = levelDimensions - offset;
		}
		
		D3D11_BOX region;
		region.left = offset.x;
		region.top = offset.y;
		region.front = 0;
		region.right = offset.x + sz.x;
		region.bottom = offset.y + sz.y;
		region.back = 1;

		// TODO: fix row size
		auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
		deviceContext->UpdateSubresource(m_handle, level, &region, data, sz.x * 4, 0);
	}

	uint32_t Dx11Texture2D::GetNumTextureLevels(const glm::u32vec2& textureSize) const
	{
		return 1u + static_cast<uint32_t>(std::floor(std::log2f(static_cast<float>(::std::max<uint32_t>(textureSize.x, textureSize.y)))));
	}

	glm::u32vec2 Dx11Texture2D::GetLevelDimensions(uint32_t level) const
	{
		glm::u32vec2 result = m_size;
		for (uint32_t i = 0; i < level; i++) {
			result.x = std::max<uint32_t>(1u, result.x / 2);
			result.y = std::max<uint32_t>(1u, result.y / 2);

			if (result.x == 1 && result.y == 1) {
				break;
			}
		}

		return result;
	}

}


