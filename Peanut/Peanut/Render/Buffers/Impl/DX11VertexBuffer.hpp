#ifndef __DX11VERTEXBUFFER_H__
#define __DX11VERTEXBUFFER_H__

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

#include <d3d11.h>

namespace pn {

class DX11VertexBuffer final : public VertexBuffer
{
public:
    DX11VertexBuffer(uint32_t size, const void* data = nullptr);
    ~DX11VertexBuffer() override;

    void Bind() override;
    void Unbind() override;
    
    void ReplaceData(uint32_t size, const void* data) override;
    void UpdateData(uint32_t size, const void* data, uint32_t offset = 0) override;
    bool GetIsDataInitialized() const override { return m_isDataInitialized; }
    
    const std::shared_ptr<BufferLayout>& GetLayout() const override;
    void SetLayout(const std::shared_ptr<BufferLayout>& layout) override;

    uint32_t GetSize() const override { return m_size; }
    uint32_t GetVertexSize() const override;
    uint32_t GetVertexCount() const override;

private:
    ID3D11Buffer* m_handle = nullptr;
    uint32_t m_size;
    std::shared_ptr<BufferLayout> m_layout;
    bool m_isDataInitialized;
};

}

#endif