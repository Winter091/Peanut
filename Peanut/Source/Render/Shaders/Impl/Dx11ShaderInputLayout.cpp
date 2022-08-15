#include "Dx11ShaderInputLayout.hpp"

#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#include <Render/Shaders/Impl/Dx11Shader.hpp>
#include <Render/Buffers/Impl/Dx11EnumConversions.hpp>

namespace pn {

Dx11ShaderInputLayout::Dx11ShaderInputLayout(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<Shader>& shader)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;
    std::vector<std::unique_ptr<std::string>> semanticNameStrings;

    for (uint32_t bufferIndex = 0; bufferIndex < vertexBuffers.size(); bufferIndex++) {
        const auto& bufferLayout = vertexBuffers[bufferIndex]->GetLayout();
        const auto& bufferDataUsage = vertexBuffers[bufferIndex]->GetDataUsage();

        for (const auto& attribute : bufferLayout->GetElements()) {

            if (attribute.GetType() == BufferLayoutElementType::Mat4) {
                for (int semanticIndex = 0; semanticIndex < 4; semanticIndex++) {
                    auto& desc = layoutDesc.emplace_back();

                    desc.SemanticName = attribute.GetName().c_str();
                    desc.SemanticIndex = semanticIndex;
                    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                    desc.InputSlot = bufferIndex;
                    desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                    desc.InputSlotClass = VertexBufferDataUsageToDx11InputSlotClass(bufferDataUsage);
                    desc.InstanceDataStepRate = VertexBufferDataUsageToDx11InstanceDataStepRate(bufferDataUsage);
                }
            }
            else {
                auto& desc = layoutDesc.emplace_back();

                desc.SemanticName = attribute.GetName().c_str();
                desc.SemanticIndex = 0;
                desc.Format = AttributeTypeToDxFormat(attribute.GetType(), attribute.GetCount());
                desc.InputSlot = bufferIndex;
                desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                desc.InputSlotClass = VertexBufferDataUsageToDx11InputSlotClass(bufferDataUsage);
                desc.InstanceDataStepRate = VertexBufferDataUsageToDx11InstanceDataStepRate(bufferDataUsage);
            }

        }
    }

    auto* device = Dx11GLFWRenderContext::GetCurrentContext().GetDevice();
    auto* dx11Shader = static_cast<Dx11Shader*>(shader.get());

    HRESULT res = device->CreateInputLayout(
        &layoutDesc[0],
        static_cast<uint32_t>(layoutDesc.size()),
        dx11Shader->GetVertexShaderBlob()->GetBufferPointer(),
        dx11Shader->GetVertexShaderBlob()->GetBufferSize(),
        m_layout.GetAddressOf()
    );

    PN_CORE_ASSERT(res == S_OK, "Unable to create input layout");
}

}
