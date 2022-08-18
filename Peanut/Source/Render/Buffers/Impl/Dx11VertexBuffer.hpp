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

    void* Map() override { return Dx11BufferImpl::Map(); }
    void Unmap() override { return Dx11BufferImpl::Unmap(); }
    void SetData(const void* data, size_t offset = 0, size_t size = 0) { Dx11BufferImpl::SetData(data, offset, size); }

    const std::shared_ptr<BufferLayout>& GetLayout() const override { return m_layout; }

    uint32_t GetSize() const { return Dx11BufferImpl::GetSize(); }
    uint32_t GetVertexSize() const override { return m_layout->GetVertexSize(); }
    uint32_t GetVertexCount() const override { return GetSize() / GetVertexSize(); }
    VertexBufferDataUsage GetDataUsage() const override{ return m_dataUsage; }

    ID3D11Buffer* GetNativeObjectPtr() const { return Dx11BufferImpl::GetNativeObjectPtr(); }

private:
    std::shared_ptr<BufferLayout> m_layout;
    VertexBufferDataUsage m_dataUsage;
};

}