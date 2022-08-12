#include "Dx11IndexBuffer.hpp"

#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/Dx11EnumConversions.hpp>

namespace pn
{

Dx11IndexBuffer::Dx11IndexBuffer(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data)
    : m_size(size)
    , m_indexCount(0u)
    , m_mapAccess(access)
    , m_format(format)
{
    PN_PROFILE_FUNCTION();

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = size;
    bufferDesc.Usage = BufferMapAccessToDxCpuUsageFlags(access);
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = BufferMapAccessToDxCpuAccessFlags(access);
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = GetDxDataTypeSize(format);

    auto* device = Dx11GLFWRenderContext::GetCurrentContext().GetDevice();
    HRESULT res = 0;

    if (!data) {
        res = device->CreateBuffer(&bufferDesc, nullptr, m_handle.GetAddressOf());
    } else {
        D3D11_SUBRESOURCE_DATA bufferSubresourceData;
        bufferSubresourceData.pSysMem = (void*)data;
        bufferSubresourceData.SysMemPitch = 0;
        bufferSubresourceData.SysMemSlicePitch = 0;

        res = device->CreateBuffer(&bufferDesc, &bufferSubresourceData, m_handle.GetAddressOf());
    }

    PN_CORE_ASSERT(res == S_OK, "Unable to create index buffer\n");

    UpdateIndexCount();
}

Dx11IndexBuffer::~Dx11IndexBuffer()
{
}

void* Dx11IndexBuffer::Map()
{
    PN_PROFILE_FUNCTION();

    auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    HRESULT res = deviceContext->Map(m_handle.Get(), 0, BufferMapAccessToDxMap(m_mapAccess), 0, &mappedBuffer);
    PN_CORE_ASSERT(res == S_OK, "Unable to map vertex buffer");

    return mappedBuffer.pData;
}

void Dx11IndexBuffer::Unmap()
{
    PN_PROFILE_FUNCTION();

    auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
    deviceContext->Unmap(m_handle.Get(), 0);
}

uint32_t Dx11IndexBuffer::GetDxDataTypeSize(IndexBufferDataFormat format) const
{
    switch (format) {
        case IndexBufferDataFormat::Uint16: return 2u;
        case IndexBufferDataFormat::Uint32: return 4u;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown index buffer data format: {}", static_cast<uint32_t>(m_format));
    return 0u;
}

void Dx11IndexBuffer::UpdateIndexCount()
{
    m_indexCount = m_size / GetDxDataTypeSize(m_format);
}

}