#pragma once

#include <Peanut/Render/Buffers/VertexArray.hpp>

#include <vector>

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

class Dx11PipelineState final : public VertexArray
{
public:
    Dx11PipelineState(const VertexArrayDescription& description);
    ~Dx11PipelineState() override;

    void Bind() override;

    uint32_t GetVertexCount() const override;
    uint32_t GetIndexCount() const override;
    IndexBufferDataFormat GetIndexDataFormat() const override;
    uint32_t GetInstanceCount() const override;

private:
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::shared_ptr<Shader> m_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

private:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> ProcessShaderInputLayout(
        std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<Shader>& shader);
};

}