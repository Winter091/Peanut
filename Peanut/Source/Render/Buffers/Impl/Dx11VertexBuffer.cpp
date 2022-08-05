#include "Dx11VertexBuffer.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include "Dx11EnumConversions.hpp"

namespace pn {

Dx11VertexBuffer::Dx11VertexBuffer(BufferMapAccess access, uint32_t size, const void* data)
	: m_handle(0)
	, m_isDataInitialized(false)
	, m_layout()
	, m_size(0)
{
	PN_CORE_ASSERT(size > 0, "Unable to create vertex buffer with size = 0");

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = BufferMapAccessToDxCpuUsageFlags(access);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = BufferMapAccessToDxCpuAccessFlags(access);
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0; // TODO: is it ok?

	D3D11_SUBRESOURCE_DATA bufferSubresourceData;
	bufferSubresourceData.pSysMem = (void*)data;
	bufferSubresourceData.SysMemPitch = 0;
	bufferSubresourceData.SysMemSlicePitch = 0;

	auto* device = Dx11GLFWRenderContext::GetCurrentContext().GetDevice();
	HRESULT res = device->CreateBuffer(&bufferDesc, &bufferSubresourceData, &m_handle);
	PN_CORE_ASSERT(res == S_OK, "Unable to create vertex buffer\n");
}

Dx11VertexBuffer::~Dx11VertexBuffer()
{
}

void Dx11VertexBuffer::Bind()
{
}

void Dx11VertexBuffer::BindToBindingIndex(uint32_t index)
{
}

void Dx11VertexBuffer::Unbind()
{
}

void* Dx11VertexBuffer::Map()
{
	return nullptr;
}

void Dx11VertexBuffer::Unmap()
{
}

const std::shared_ptr<BufferLayout>& Dx11VertexBuffer::GetLayout() const
{
	return m_layout;
}

void Dx11VertexBuffer::SetLayout(const std::shared_ptr<BufferLayout>& layout)
{
}

uint32_t Dx11VertexBuffer::GetVertexSize() const
{
	return 0;
}

uint32_t Dx11VertexBuffer::GetVertexCount() const
{
	return 0;
}

}