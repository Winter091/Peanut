#include "Dx11BufferImpl.hpp"

#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include "Dx11EnumConversions.hpp"

#include <d3d11.h>

namespace pn {

Dx11BufferImpl::Dx11BufferImpl(BufferMapAccess access, uint32_t size)
	: m_size(size)
	, m_mapAccess(access)
{}

void Dx11BufferImpl::Init(ID3D11Buffer** handlePtr, const D3D11_BUFFER_DESC& desc, const void* data)
{
	auto* device = Dx11GLFWRenderContext::GetCurrentContext().GetDevice();
	HRESULT res = 0;

	if (!data) {
		res = device->CreateBuffer(&desc, nullptr, handlePtr);
	}
	else {
		D3D11_SUBRESOURCE_DATA bufferSubresourceData;
		bufferSubresourceData.pSysMem = (void*)data;
		bufferSubresourceData.SysMemPitch = 0;
		bufferSubresourceData.SysMemSlicePitch = 0;

		res = device->CreateBuffer(&desc, &bufferSubresourceData, handlePtr);
	}

	PN_CORE_ASSERT(res == S_OK, "Unable to create buffer\n");
}

void* Dx11BufferImpl::Map(ID3D11Buffer* handle)
{
	PN_PROFILE_FUNCTION();

	auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	HRESULT res = deviceContext->Map(handle, 0, BufferMapAccessToDxMap(m_mapAccess), 0, &mappedBuffer);
	PN_CORE_ASSERT(res == S_OK, "Unable to map buffer");

	return mappedBuffer.pData;
}
void Dx11BufferImpl::Unmap(ID3D11Buffer* handle)
{
	PN_PROFILE_FUNCTION();

	auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
	deviceContext->Unmap(handle, 0);
}

}
