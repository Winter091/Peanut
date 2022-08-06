#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

class Dx11VertexBuffer final : public VertexBuffer
{
public:
    Dx11VertexBuffer(BufferMapAccess access, uint32_t size, const std::shared_ptr<BufferLayout>& layout, const void* data);
    ~Dx11VertexBuffer() override;

    void* Map() override;
    void Unmap() override;

    const std::shared_ptr<BufferLayout>& GetLayout() const override;

    uint32_t GetSize() const { return m_size; }
    uint32_t GetVertexSize() const override;
    uint32_t GetVertexCount() const override;

    void* GetNativeHandle() const override { return (void*)m_handle.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_handle = nullptr;
    uint32_t m_size;
    std::shared_ptr<BufferLayout> m_layout;
    bool m_isDataInitialized;
    BufferMapAccess m_mapAccess;
};

}