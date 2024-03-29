#pragma once

#include <Peanut/Render/Buffers/VertexArray.hpp>

#include <vector>

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

    class Dx11VertexArray final : public VertexArray
    {
    public:
        Dx11VertexArray(const VertexArrayDescription& description);
        ~Dx11VertexArray() override = default;

        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; }
        virtual std::shared_ptr<ShaderInputLayout> GetShaderInputLayout() const override { return m_inputLayout; }

        std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_indexBuffer; }
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

        uint32_t GetVertexCount() const override;
        uint32_t GetIndexCount() const override;
        uint32_t GetInstanceCount() const override;
        IndexBufferDataFormat GetIndexDataFormat() const override;

    private:
        void UpdateInstanceCount(const VertexBuffer& vertexBuffer);

    private:
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<ShaderInputLayout> m_inputLayout;
        uint32_t m_instanceCount = 0u;
    };

}
