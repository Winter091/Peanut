#include "Dx11VertexBuffer.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include "Dx11EnumConversions.hpp"

using Microsoft::WRL::ComPtr;

namespace pn {

Dx11VertexBuffer::Dx11VertexBuffer(BufferMapAccess access, uint32_t size, const std::shared_ptr<BufferLayout>& layout, const void* data)
	: m_handle(0)
	, m_isDataInitialized(false)
	, m_layout(layout)
	, m_size(0)
	, m_mapAccess(access)
{
	PN_CORE_ASSERT(size > 0, "Unable to create vertex buffer with size = 0");

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = BufferMapAccessToDxCpuUsageFlags(access);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = BufferMapAccessToDxCpuAccessFlags(access);
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = m_layout->GetStride();

	D3D11_SUBRESOURCE_DATA bufferSubresourceData;
	bufferSubresourceData.pSysMem = (void*)data;
	bufferSubresourceData.SysMemPitch = 0;
	bufferSubresourceData.SysMemSlicePitch = 0;

	auto* device = Dx11GLFWRenderContext::GetCurrentContext().GetDevice();
	HRESULT res = device->CreateBuffer(&bufferDesc, &bufferSubresourceData, m_handle.GetAddressOf());
	PN_CORE_ASSERT(res == S_OK, "Unable to create vertex buffer\n");
}

Dx11VertexBuffer::~Dx11VertexBuffer()
{
}

void* Dx11VertexBuffer::Map()
{
	auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	HRESULT res = deviceContext->Map(m_handle.Get(), 0, BufferMapAccessToDxMap(m_mapAccess), 0, &mappedBuffer);
	PN_CORE_ASSERT(res == S_OK, "Unable to map vertex buffer");

	return mappedBuffer.pData;

}

void Dx11VertexBuffer::Unmap()
{
	auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
	deviceContext->Unmap(m_handle.Get(), 0);
}

const std::shared_ptr<BufferLayout>& Dx11VertexBuffer::GetLayout() const
{
	return m_layout;
}

uint32_t Dx11VertexBuffer::GetVertexSize() const
{
	return m_layout->GetStride();
}

uint32_t Dx11VertexBuffer::GetVertexCount() const
{
	return m_size / GetVertexSize();
}

}