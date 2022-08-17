#pragma once

#include <Peanut/Render/Buffers/IndexBuffer.hpp>
#include "Dx11BufferImpl.hpp"

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

class Dx11IndexBuffer final : public IndexBuffer, public Dx11BufferImpl
{
public:
    Dx11IndexBuffer(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data = nullptr);
    ~Dx11IndexBuffer() override = default;

    void* Map() override { return Dx11BufferImpl::Map(m_handle.Get()); }
    void Unmap() override { return Dx11BufferImpl::Unmap(m_handle.Get()); }

    uint32_t GetSize() const override { return Dx11BufferImpl::GetSize(); }
    uint32_t GetIndexCount() const override { return m_indexCount; }
    IndexBufferDataFormat GetDataFormat() const override { return m_format; }

    ID3D11Buffer* GetNativeObjectPtr() const { return m_handle.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_handle = nullptr;
    uint32_t m_indexCount;
    IndexBufferDataFormat m_format;
};

}
