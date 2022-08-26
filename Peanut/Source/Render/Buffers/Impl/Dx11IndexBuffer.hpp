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

        void* Map() override { return Dx11BufferImpl::Map(); }
        void Unmap() override { return Dx11BufferImpl::Unmap(); }
        void SetData(const void* data, size_t offset = 0, size_t size = 0) { Dx11BufferImpl::SetData(data, offset, size); }

        uint32_t GetSize() const override { return Dx11BufferImpl::GetSize(); }
        uint32_t GetIndexCount() const override { return m_indexCount; }
        IndexBufferDataFormat GetDataFormat() const override { return m_format; }

        ID3D11Buffer* GetNativeObjectPtr() const { return Dx11BufferImpl::GetNativeObjectPtr(); }

    private:
        uint32_t m_indexCount;
        IndexBufferDataFormat m_format;
    };

}
