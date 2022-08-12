#include "Dx11PipelineState.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include <Render/Shaders/Impl/Dx11Shader.hpp>
#include <Render/Shaders/Impl/Dx11ShaderInputLayout.hpp>
#include "Dx11EnumConversions.hpp"

#include <unordered_set>

namespace pn
{

Dx11PipelineState::Dx11PipelineState(const PipelineStateDescription& description)
    : m_vertexBuffers(description.VertexBuffers)
    , m_indexBuffer(description.IndexBuffer)
    , m_constantBuffers(description.ConstantBuffers)
    , m_shader(description.Shader)
    , m_inputLayout(description.ShaderInputLayout)
{
}

Dx11PipelineState::~Dx11PipelineState()
{
}

void Dx11PipelineState::Bind()
{
    auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();

    std::vector<ID3D11Buffer*> vertexBuffers;
    std::vector<uint32_t> strides;
    std::vector<uint32_t> offsets(m_vertexBuffers.size(), 0);

    for (const auto& vertexBuffer : m_vertexBuffers) {
        vertexBuffers.push_back((ID3D11Buffer*)vertexBuffer->GetNativeHandle());
        strides.push_back(vertexBuffer->GetLayout()->GetStride());
    }

    deviceContext->IASetVertexBuffers(0, static_cast<uint32_t>(vertexBuffers.size()), &vertexBuffers[0], &strides[0], &offsets[0]);
    deviceContext->IASetInputLayout(dynamic_cast<Dx11ShaderInputLayout*>(m_inputLayout.get())->GetLayout());

    if (m_indexBuffer) {
        deviceContext->IASetIndexBuffer((ID3D11Buffer*)m_indexBuffer->GetNativeHandle(), IndexBufferFormatToDx11Format(m_indexBuffer->GetDataFormat()), 0);
    }

    if (!m_constantBuffers.empty()) {
        std::vector<ID3D11Buffer*> constantBuffers;
        
        for (const auto& constantBuffer : m_constantBuffers) {
            constantBuffers.push_back((ID3D11Buffer*)constantBuffer->GetNativeHandle());
        }

        deviceContext->VSSetConstantBuffers(0, static_cast<uint32_t>(constantBuffers.size()), &constantBuffers[0]);
        deviceContext->PSSetConstantBuffers(0, static_cast<uint32_t>(constantBuffers.size()), &constantBuffers[0]);
    }

    m_shader->Bind();
}

void Dx11PipelineState::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    m_indexBuffer = indexBuffer;
}

void Dx11PipelineState::SetConstantBuffers(const std::vector<std::shared_ptr<ConstantBuffer>>& constantBuffers)
{
    m_constantBuffers = constantBuffers;
}

uint32_t Dx11PipelineState::GetVertexCount() const
{
    PN_CORE_ASSERT(!m_vertexBuffers.empty(), "No vertex buffers are bound to pipeline state, can't get vertex count");
    return m_vertexBuffers.front()->GetVertexCount();
}

uint32_t Dx11PipelineState::GetIndexCount() const
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set, unable to get its data format");
    return m_indexBuffer->GetIndexCount();
}

uint32_t Dx11PipelineState::GetInstanceCount() const
{
    // TODO: fixme
    return 0;
}

IndexBufferDataFormat Dx11PipelineState::GetIndexDataFormat() const
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set, unable to get its data format");
    return m_indexBuffer->GetDataFormat();
}

}