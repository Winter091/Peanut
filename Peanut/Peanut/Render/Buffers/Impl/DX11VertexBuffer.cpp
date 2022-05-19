#include "DX11VertexBuffer.hpp"

#include <Peanut/Window/RenderContext/Impl/DX11GLFWRenderContext.hpp>
#include <Peanut/Core/Assert.hpp>

namespace pn
{

DX11VertexBuffer::DX11VertexBuffer(uint32_t size, const void* data)
{
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = size;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;

    // TODO: Can data even be nullptr?
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = data;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    ID3D11Device* device = DX11GLFWRenderContext::GetCurrentContext().GetDevice();
    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &m_handle);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "Failed to create vertex buffer");
    }

    m_isDataInitialized = (data != nullptr);
}

DX11VertexBuffer::~DX11VertexBuffer()
{
    m_handle->Release();
}

void DX11VertexBuffer::Bind()
{
    uint32_t stride = m_layout->GetStride();
    uint32_t offset = 0;

    ID3D11DeviceContext* context = DX11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
    context->IASetVertexBuffers(0, 1, &m_handle, &stride, &offset);
}

void DX11VertexBuffer::Unbind()
{
    ID3D11DeviceContext* context = DX11GLFWRenderContext::GetCurrentContext().GetDeviceContext();
    context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
}

void DX11VertexBuffer::ReplaceData(uint32_t size, const void* data)
{
    PN_CORE_ASSERT(false, "ReplaceData is not implemented");
}

void DX11VertexBuffer::UpdateData(uint32_t size, const void* data, uint32_t offset)
{
    PN_CORE_ASSERT(false, "UpdateData is not implemented");
}

const std::shared_ptr<BufferLayout>& DX11VertexBuffer::GetLayout() const 
{
    PN_CORE_ASSERT(m_layout, "Layout is not specified");
    return m_layout;
}

void DX11VertexBuffer::SetLayout(const std::shared_ptr<BufferLayout>& layout)
{
    PN_CORE_ASSERT(layout, "Layout is nullptr");
    m_layout = layout;
}

uint32_t DX11VertexBuffer::GetVertexSize() const 
{
    PN_CORE_ASSERT(m_layout, "Layout is nullptr");
    return m_layout->GetStride();
}

uint32_t DX11VertexBuffer::GetVertexCount() const 
{
    PN_CORE_ASSERT(m_layout, "Layout is nullptr");
    return GetSize() / GetVertexSize();
}

}