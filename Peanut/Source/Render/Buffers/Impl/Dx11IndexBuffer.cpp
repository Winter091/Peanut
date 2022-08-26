#include "Dx11IndexBuffer.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/Dx11EnumConversions.hpp>

namespace pn
{

Dx11IndexBuffer::Dx11IndexBuffer(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data)
    : Dx11BufferImpl(access, size)
    , m_indexCount(size / GetIndexBufferDataFormatSize(m_format))
    , m_format(format)
{
    PN_PROFILE_FUNCTION();

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = size;
    desc.Usage = BufferMapAccessToDxCpuUsageFlags(access);
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = BufferMapAccessToDxCpuAccessFlags(access);
    desc.MiscFlags = 0;
    desc.StructureByteStride = GetIndexBufferDataFormatSize(format);

    Dx11BufferImpl::Init(desc, data);
}

}