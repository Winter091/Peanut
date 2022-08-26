#include "Dx11BufferImpl.hpp"
#include "Dx11BufferImpl.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Window/RenderContextImpl/Dx11RenderContext.hpp>
#include "Dx11EnumConversions.hpp"

#include <d3d11.h>

namespace pn {

	Dx11BufferImpl::Dx11BufferImpl(BufferMapAccess access, uint32_t size)
		: m_size(size)
		, m_mapAccess(access)
	{}

	Dx11BufferImpl::~Dx11BufferImpl()
	{
		PN_CORE_ASSERT(m_handle, "Buffer is not initialized");
		m_handle->Release();
	}

	void Dx11BufferImpl::Init(const D3D11_BUFFER_DESC& desc, const void* data)
	{
		PN_CORE_ASSERT(!m_handle, "Buffer is already initialized");

		auto* device = Dx11RenderContext::GetCurrentContext().GetDevice();
		HRESULT res = 0;

		if (!data) {
			res = device->CreateBuffer(&desc, nullptr, &m_handle);
		}
		else {
			D3D11_SUBRESOURCE_DATA bufferSubresourceData;
			bufferSubresourceData.pSysMem = (void*)data;
			bufferSubresourceData.SysMemPitch = 0;
			bufferSubresourceData.SysMemSlicePitch = 0;

			res = device->CreateBuffer(&desc, &bufferSubresourceData, &m_handle);
		}

		PN_CORE_ASSERT(res == S_OK, "Unable to create buffer\n");
	}

	void* Dx11BufferImpl::Map()
	{
		PN_CORE_ASSERT(m_handle, "Buffer is not initialized");
		PN_PROFILE_FUNCTION();

		auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT res = deviceContext->Map(m_handle, 0, BufferMapAccessToDxMap(m_mapAccess), 0, &mappedBuffer);
		PN_CORE_ASSERT(res == S_OK, "Unable to map buffer");

		return mappedBuffer.pData;
	}
	void Dx11BufferImpl::Unmap()
	{
		PN_CORE_ASSERT(m_handle, "Buffer is already initialized");
		PN_PROFILE_FUNCTION();

		auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
		deviceContext->Unmap(m_handle, 0);
	}

	void Dx11BufferImpl::SetData(const void* data, size_t offset, size_t size)
	{
		PN_CORE_ASSERT(offset + size <= m_size, "Trying to set data outsize of buffer data range: buffer size = {}, "
					   "trying to set size = {} with offset = {}", m_size, size, offset);
	
		if (size == 0) {
			size = m_size - offset;
		}
	
		uint8_t* mappedData = reinterpret_cast<uint8_t*>(Map());
		memcpy(mappedData + offset, data, size);
		Unmap();
	}

}
