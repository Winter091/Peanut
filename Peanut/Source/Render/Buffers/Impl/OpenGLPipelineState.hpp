#pragma once

#include <Peanut/Render/Buffers/PipelineState.hpp>

#include <vector>

namespace pn {

class OpenGLPipelineState final : public PipelineState
{
public:
    OpenGLPipelineState(const PipelineStateDescription& description);
    ~OpenGLPipelineState() override;

    void Bind() override;

    const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; }
    std::shared_ptr<Shader> GetShader() const override { return m_shader; }
    std::shared_ptr<ShaderInputLayout> GetShaderInputLayout() const override { return nullptr; }

    std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_indexBuffer; }
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    const std::vector<std::shared_ptr<ConstantBuffer>>& GetConstantBuffers() const override { return m_constantBuffers; }
    void SetConstantBuffers(const std::vector<std::shared_ptr<ConstantBuffer>>& constantBuffers) override;

    uint32_t GetVertexCount() const override;
    uint32_t GetIndexCount() const override;
    uint32_t GetInstanceCount() const override;
    IndexBufferDataFormat GetIndexDataFormat() const override;

private:
    uint32_t m_vaoHandle = 0u;
    uint32_t m_instanceCount = 0u;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::vector<std::shared_ptr<ConstantBuffer>> m_constantBuffers;
    std::shared_ptr<Shader> m_shader;

private:
    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
    void ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, uint32_t bindingIndex);
    void AssertAllAttributeIndicesAreUnique() const;
    void UpdateInstanceCount(const VertexBuffer& vertexBuffer);
};

}
