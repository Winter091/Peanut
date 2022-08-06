#pragma once

#include <Peanut/Render/Buffers/IndexBuffer.hpp>

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

class Dx11IndexBuffer final : public IndexBuffer
{
public:
    Dx11IndexBuffer(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data = nullptr);
    ~Dx11IndexBuffer() override;

    void* Map() override;
    void Unmap() override;

    uint32_t GetIndexCount() const override { return m_indexCount; }
    IndexBufferDataFormat GetDataFormat() const override { return m_format; }

    void* GetNativeHandle() const override { return (void*)m_handle.Get(); }

private:
    void UpdateIndexCount();
    uint32_t GetDxDataTypeSize(IndexBufferDataFormat format) const;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_handle = nullptr;
    uint32_t m_size;
    uint32_t m_indexCount;
    IndexBufferDataFormat m_format;
    BufferMapAccess m_mapAccess;
};

}
