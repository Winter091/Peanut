#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include "Dx11BufferImpl.hpp"

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

class Dx11VertexBuffer final : public VertexBuffer, public Dx11BufferImpl
{
public:
    Dx11VertexBuffer(const std::shared_ptr<BufferLayout>& layout, BufferMapAccess access,
        VertexBufferDataUsage dataUsage, uint32_t size, const void* data);
    ~Dx11VertexBuffer() override = default;

    void* Map() override { return Dx11BufferImpl::Map(m_handle.Get()); }
    void Unmap() override { return Dx11BufferImpl::Unmap(m_handle.Get()); }
    void SetData(const void* data, size_t offset = 0, size_t size = 0) { Dx11BufferImpl::SetData(m_handle.Get(), data, offset, size); }

    const std::shared_ptr<BufferLayout>& GetLayout() const override { return m_layout; }

    uint32_t GetSize() const { return Dx11BufferImpl::GetSize(); }
    uint32_t GetVertexSize() const override { return m_layout->GetVertexSize(); }
    uint32_t GetVertexCount() const override { return GetSize() / GetVertexSize(); }
    VertexBufferDataUsage GetDataUsage() const override{ return m_dataUsage; }

    ID3D11Buffer* GetNativeObjectPtr() const { return m_handle.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_handle = nullptr;
    std::shared_ptr<BufferLayout> m_layout;
    VertexBufferDataUsage m_dataUsage;
};

}