#include "Dx11TextureSampler.hpp"

#include <Window/RenderContextImpl/Dx11RenderContext.hpp>
#include "Dx11EnumConversions.hpp"

#include <d3d11.h>

namespace pn {

	Dx11TextureSampler::Dx11TextureSampler(const TextureSamplerSettings& settings)
	{
		D3D11_SAMPLER_DESC desc;
		desc.Filter = GetDx11Filter(settings.GetFilter());
		desc.AddressU = GetDx11AddressMode(settings.GetWrapX());
		desc.AddressV = GetDx11AddressMode(settings.GetWrapY());;
		desc.AddressW = GetDx11AddressMode(settings.GetWrapZ());;
		desc.MipLODBias = settings.GetLodBias();
		desc.MaxAnisotropy = settings.GetMaxAnisotropy();
		desc.ComparisonFunc = GetDx11ComparisonFunc(settings.GetComparisonFunc());
		memcpy(&desc.BorderColor[0], &settings.GetBorderColor()[0], 4 * sizeof(float));
		desc.MinLOD = settings.GetMinLod();
		desc.MaxLOD = settings.GetMaxLod();

		auto* device = Dx11RenderContext::GetCurrentContext().GetDevice();
		HRESULT res = device->CreateSamplerState(&desc, &m_handle);
		
		PN_CORE_ASSERT(res == S_OK, "Unable to create texture sampler state");
	}

	Dx11TextureSampler::~Dx11TextureSampler()
	{
		m_handle->Release();
	}

}
