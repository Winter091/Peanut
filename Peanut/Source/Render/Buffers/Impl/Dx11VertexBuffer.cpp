#include "Dx11VertexBuffer.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include "Dx11EnumConversions.hpp"

using Microsoft::WRL::ComPtr;

namespace pn {

Dx11VertexBuffer::Dx11VertexBuffer(const std::shared_ptr<BufferLayout>& layout, BufferMapAccess access,
								   VertexBufferDataUsage dataUsage, uint32_t size, const void* data)
	: Dx11BufferImpl(access, size)
	, m_layout(layout)
	, m_dataUsage(dataUsage)
{
	PN_PROFILE_FUNCTION();

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.Usage = BufferMapAccessToDxCpuUsageFlags(access);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = BufferMapAccessToDxCpuAccessFlags(access);
	desc.MiscFlags = 0;
	desc.StructureByteStride = m_layout->GetVertexSize();

	Dx11BufferImpl::Init(m_handle.GetAddressOf(), desc, data);
}

}