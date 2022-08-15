#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

class Dx11VertexBuffer final : public VertexBuffer
{
public:
    Dx11VertexBuffer(const std::shared_ptr<BufferLayout>& layout, BufferMapAccess access,
        VertexBufferDataUsage dataUsage, uint32_t size, const void* data);
    ~Dx11VertexBuffer() override = default;

    void* Map() override;
    void Unmap() override;

    const std::shared_ptr<BufferLayout>& GetLayout() const override { return m_layout; }

    uint32_t GetSize() const { return m_size; }
    uint32_t GetVertexSize() const override { return m_layout->GetVertexSize(); }
    uint32_t GetVertexCount() const override { return GetSize() / GetVertexSize(); }
    VertexBufferDataUsage GetDataUsage() const override{ return m_dataUsage; }

    ID3D11Buffer* GetNativeObjectPtr() const { return m_handle.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_handle = nullptr;
    std::shared_ptr<BufferLayout> m_layout;
    uint32_t m_size;
    BufferMapAccess m_mapAccess;
    VertexBufferDataUsage m_dataUsage;
};

}