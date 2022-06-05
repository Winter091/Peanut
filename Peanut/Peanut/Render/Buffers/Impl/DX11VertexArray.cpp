#include "DX11VertexArray.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Window/RenderContext/Impl/DX11GLFWRenderContext.hpp>

#include <d3d11.h>
#include <vector>

namespace pn
{

DX11VertexArray::DX11VertexArray()
{
    
}

DX11VertexArray::~DX11VertexArray()
{
    if (m_vertexLayout) {
        m_vertexLayout->Release();
    }
}

void DX11VertexArray::Bind()
{
    PN_CORE_ASSERT(m_vertexBuffer, "Vertex buffer is not set");
    m_vertexBuffer->Bind();
}

void DX11VertexArray::Unbind()
{
    PN_CORE_ASSERT(m_vertexBuffer, "Vertex buffer is not set");
    m_vertexBuffer->Unbind();
}

void DX11VertexArray::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    m_vertexBuffer = vertexBuffer;
    ProcessVertexBufferLayout();
}

void DX11VertexArray::ProcessVertexBufferLayout()
{
    const auto& layout = m_vertexBuffer->GetLayout();
    const auto& layoutElements = layout->GetElements();

    std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
    elements.reserve(layoutElements.size());

    for (uint32_t i = 0; i < layoutElements.size(); i++) {
        auto elem = ProcessVertexBufferLayoutElement(layoutElements[i], i, layout->GetStride());
        elements.push_back(elem);
    }

    const auto& device = DX11GLFWRenderContext::GetCurrentContext().GetDevice();
    device->CreateInputLayout(&elements[0], elements.size(), nullptr, 0, &m_vertexLayout);
}

D3D11_INPUT_ELEMENT_DESC DX11VertexArray::ProcessVertexBufferLayoutElement(
    const BufferLayoutElement& elem, uint32_t index, uint32_t stride
)
{
    D3D11_INPUT_ELEMENT_DESC description;
    description.SemanticName = elem.name.c_str();
    description.SemanticIndex = index;
    description.Format = MapTypeToDXFormat(elem.type);
    description.InputSlot = index;
    description.AlignedByteOffset = (index == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT);
    description.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    description.InstanceDataStepRate = 0;

    return description;
}

DXGI_FORMAT DX11VertexArray::MapTypeToDXFormat(BufferLayoutElementType type) const
{
    PN_CORE_ASSERT(false, "Unknown element type: {}", static_cast<uint32_t>(type));
    return DXGI_FORMAT_UNKNOWN;
}

void DX11VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    PN_CORE_ASSERT(false, "No index buffers on dx11");
}

uint32_t DX11VertexArray::GetSize() const 
{
    PN_CORE_ASSERT(m_vertexBuffer, "Vertex buffer is not set");
    return m_vertexBuffer->GetSize();
}

uint32_t DX11VertexArray::GetVertexCount() const 
{
    PN_CORE_ASSERT(m_vertexBuffer, "Vertex buffer is not set");
    return m_vertexBuffer->GetVertexCount();
}

uint32_t DX11VertexArray::GetIndexCount() const 
{
    PN_CORE_ASSERT(false, "No index buffers on dx11");
    return 0;
}

IndexBufferDataFormat DX11VertexArray::GetIndexDataFormat() const 
{
    PN_CORE_ASSERT(false, "No index buffers on dx11");
    return IndexBufferDataFormat::Uint8;
}

bool DX11VertexArray::GetIsVertexBufferInitialized() const 
{
    return m_vertexBuffer && m_vertexBuffer->GetIsDataInitialized();
}

bool DX11VertexArray::GetIsIndexBufferInitialized() const 
{
    return m_indexBuffer && m_indexBuffer->GetIsDataInitialized();
}

}