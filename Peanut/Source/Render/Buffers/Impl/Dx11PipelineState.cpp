#include "Dx11PipelineState.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include <Render/Shaders/Impl/Dx11Shader.hpp>
#include "Dx11EnumConversions.hpp"

#include <unordered_set>

namespace pn
{

Dx11PipelineState::Dx11PipelineState(const VertexArrayDescription& description)
    : m_vertexBuffers(description.VertexBuffers)
    , m_indexBuffer(description.IndexBuffer)
    , m_shader(description.Shader)
    , m_inputLayout(nullptr)
{
    m_inputLayout = ProcessShaderInputLayout(m_vertexBuffers, m_shader);
}

Microsoft::WRL::ComPtr<ID3D11InputLayout> Dx11PipelineState::ProcessShaderInputLayout(
    std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<Shader>& shader)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;
    for (uint32_t bufferIndex = 0; bufferIndex < vertexBuffers.size(); bufferIndex++) {
        const auto& bufferLayout = vertexBuffers[bufferIndex]->GetLayout();

        for (const auto& attribute : bufferLayout->GetElements()) {
            auto& desc = layoutDesc.emplace_back();

            desc.SemanticName = attribute.name.c_str();
            desc.SemanticIndex = 0;
            desc.Format = AttributeTypeToDxFormat(attribute.type, attribute.count);
            desc.InputSlot = bufferIndex;
            desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
            desc.InputSlotClass = BufferLayoutAttributeUsageToDx11InputSlotClass(bufferLayout->GetUsage());
            desc.InstanceDataStepRate = BufferLayoutAttributeUsageToDx11InstanceDataStepRate(bufferLayout->GetUsage());
        }
    }

    auto* device = Dx11GLFWRenderContext::GetCurrentContext().GetDevice();
    auto* dx11Shader = dynamic_cast<Dx11Shader*>(shader.get());

    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    HRESULT res = device->CreateInputLayout(
        &layoutDesc[0],
        static_cast<uint32_t>(layoutDesc.size()),
        dx11Shader->GetVertexShaderBlob()->GetBufferPointer(),
        dx11Shader->GetVertexShaderBlob()->GetBufferSize(),
        inputLayout.GetAddressOf()
    );

    PN_CORE_ASSERT(res == S_OK, "Unable to create input layout");
    return inputLayout;
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
    deviceContext->IASetInputLayout(m_inputLayout.Get());

    if (m_indexBuffer) {
        deviceContext->IASetIndexBuffer((ID3D11Buffer*)m_indexBuffer->GetNativeHandle(), IndexBufferFormatToDx11Format(m_indexBuffer->GetDataFormat()), 0);
    }

    m_shader->Bind();
}

uint32_t Dx11PipelineState::GetVertexCount() const
{
    PN_CORE_ASSERT(!m_vertexBuffers.empty(), "No vertex buffers are bound to vertex array, can't get vertex count");
    return m_vertexBuffers.front()->GetVertexCount();
}

uint32_t Dx11PipelineState::GetIndexCount() const
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set, unable to get its data format");
    return m_indexBuffer->GetIndexCount();
}

IndexBufferDataFormat Dx11PipelineState::GetIndexDataFormat() const
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set, unable to get its data format");
    return m_indexBuffer->GetDataFormat();
}

uint32_t Dx11PipelineState::GetInstanceCount() const
{
    // TODO: fixme
    return 0;
}

}