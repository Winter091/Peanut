#include "Dx11VertexArray.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include <Render/Shaders/Impl/Dx11Shader.hpp>
#include <Render/Shaders/Impl/Dx11ShaderInputLayout.hpp>
#include "Dx11EnumConversions.hpp"
#include "Dx11VertexBuffer.hpp"
#include "Dx11IndexBuffer.hpp"
#include "Dx11ConstantBuffer.hpp"

#include <unordered_set>
#include <algorithm>

namespace pn
{

Dx11VertexArray::Dx11VertexArray(const VertexArrayDescription& description)
    : m_vertexBuffers(description.VertexBuffers)
    , m_indexBuffer(description._IndexBuffer)
    , m_constantBuffers(description.ConstantBuffers)
    , m_shader(description._Shader)
    , m_inputLayout(description._ShaderInputLayout)
{
    for (const auto& vertexBuffer : m_vertexBuffers) {
        if (vertexBuffer->GetDataUsage() == VertexBufferDataUsage::PerInstance) {
            UpdateInstanceCount(*vertexBuffer);
        }
    }
}

void Dx11VertexArray::Bind()
{
    auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();

    std::vector<ID3D11Buffer*> vertexBuffers;
    std::vector<uint32_t> strides;
    std::vector<uint32_t> offsets(m_vertexBuffers.size(), 0);

    for (const auto& vertexBuffer : m_vertexBuffers) {
        vertexBuffers.push_back(static_cast<Dx11VertexBuffer&>(*vertexBuffer).GetNativeObjectPtr());
        strides.push_back(vertexBuffer->GetLayout()->GetVertexSize());
    }

    deviceContext->IASetVertexBuffers(0, static_cast<uint32_t>(vertexBuffers.size()), &vertexBuffers[0], &strides[0], &offsets[0]);
    deviceContext->IASetInputLayout(static_cast<Dx11ShaderInputLayout&>(*m_inputLayout).Get());

    if (m_indexBuffer) {
        deviceContext->IASetIndexBuffer(static_cast<Dx11IndexBuffer&>(*m_indexBuffer).GetNativeObjectPtr(), IndexBufferFormatToDx11Format(m_indexBuffer->GetDataFormat()), 0);
    }

    if (!m_constantBuffers.empty()) {
        std::vector<ID3D11Buffer*> constantBuffers;
        
        for (const auto& constantBuffer : m_constantBuffers) {
            constantBuffers.push_back(static_cast<Dx11ConstantBuffer&>(*constantBuffer).GetNativeObjectPtr());
        }

        deviceContext->VSSetConstantBuffers(0, static_cast<uint32_t>(constantBuffers.size()), &constantBuffers[0]);
        deviceContext->PSSetConstantBuffers(0, static_cast<uint32_t>(constantBuffers.size()), &constantBuffers[0]);
    }

    m_shader->Bind();
}

void Dx11VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    m_indexBuffer = indexBuffer;
}

void Dx11VertexArray::SetConstantBuffers(const std::vector<std::shared_ptr<ConstantBuffer>>& constantBuffers)
{
    m_constantBuffers = constantBuffers;
}

uint32_t Dx11VertexArray::GetVertexCount() const
{
    PN_CORE_ASSERT(!m_vertexBuffers.empty(), "No vertex buffers are bound to pipeline state, can't get vertex count");
    return m_vertexBuffers.front()->GetVertexCount();
}

uint32_t Dx11VertexArray::GetIndexCount() const
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set, unable to get its data format");
    return m_indexBuffer->GetIndexCount();
}

uint32_t Dx11VertexArray::GetInstanceCount() const
{
    // TODO: fixme
    return 0;
}

IndexBufferDataFormat Dx11VertexArray::GetIndexDataFormat() const
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set, unable to get its data format");
    return m_indexBuffer->GetDataFormat();
}

void Dx11VertexArray::UpdateInstanceCount(const VertexBuffer& vertexBuffer)
{
    if (m_instanceCount == 0) {
        m_instanceCount = vertexBuffer.GetVertexCount();
    } else {
        m_instanceCount = std::min<uint32_t>(m_instanceCount, vertexBuffer.GetVertexCount());
    }
}

}