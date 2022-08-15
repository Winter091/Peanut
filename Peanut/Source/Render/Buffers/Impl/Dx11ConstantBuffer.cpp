#include "Dx11ConstantBuffer.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/Dx11EnumConversions.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>

namespace pn
{

Dx11ConstantBuffer::Dx11ConstantBuffer(BufferMapAccess access, uint32_t size, const void* data)
    : m_size(size)
    , m_mapAccess(access)
{
    PN_PROFILE_FUNCTION();

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = size;
    bufferDesc.Usage = BufferMapAccessToDxCpuUsageFlags(access);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = BufferMapAccessToDxCpuAccessFlags(access);
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

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

    PN_CORE_ASSERT(res == S_OK, "Unable to create buffer\n");
}

void* Dx11ConstantBuffer::Map()
{
    PN_PROFILE_FUNCTION();

    auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    HRESULT res = deviceContext->Map(m_handle.Get(), 0, BufferMapAccessToDxMap(m_mapAccess), 0, &mappedBuffer);
    PN_CORE_ASSERT(res == S_OK, "Unable to map vertex buffer");

    return mappedBuffer.pData;
}

void Dx11ConstantBuffer::Unmap()
{
    PN_PROFILE_FUNCTION();

    auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
    deviceContext->Unmap(m_handle.Get(), 0);
}

}