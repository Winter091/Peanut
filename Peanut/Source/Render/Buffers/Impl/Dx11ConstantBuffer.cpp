#include "Dx11ConstantBuffer.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/Dx11EnumConversions.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>

namespace pn
{

Dx11ConstantBuffer::Dx11ConstantBuffer(BufferMapAccess access, uint32_t size, const void* data)
    : Dx11BufferImpl(access, size)
{
    PN_PROFILE_FUNCTION();

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = size;
    desc.Usage = BufferMapAccessToDxCpuUsageFlags(access);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = BufferMapAccessToDxCpuAccessFlags(access);
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    Dx11BufferImpl::Init(m_handle.GetAddressOf(), desc, data);
}

}