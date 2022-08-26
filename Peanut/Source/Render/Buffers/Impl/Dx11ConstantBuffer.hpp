#pragma once

#include <Peanut/Render/Buffers/ConstantBuffer.hpp>
#include "Dx11BufferImpl.hpp"

#include <d3d11.h>

namespace pn {

    class Dx11ConstantBuffer final : public ConstantBuffer, public Dx11BufferImpl
    {
    public:
        Dx11ConstantBuffer(BufferMapAccess access, uint32_t size, const void* data);
        ~Dx11ConstantBuffer() override = default;

        void* Map() override { return Dx11BufferImpl::Map(); }
        void Unmap() override { return Dx11BufferImpl::Unmap(); }
        void SetData(const void* data, size_t offset = 0, size_t size = 0) { Dx11BufferImpl::SetData(data, offset, size); }
    
        uint32_t GetSize() const override { return Dx11BufferImpl::GetSize(); }
        ID3D11Buffer* GetNativeObjectPtr() const { return Dx11BufferImpl::GetNativeObjectPtr(); }
    };

}
