#pragma once

#include <Peanut/Render/Buffers/ConstantBuffer.hpp>
#include "Dx11BufferImpl.hpp"

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

class Dx11ConstantBuffer final : public ConstantBuffer, public Dx11BufferImpl
{
public:
    Dx11ConstantBuffer(BufferMapAccess access, uint32_t size, const void* data);
    ~Dx11ConstantBuffer() override = default;

    void* Map() override { return Dx11BufferImpl::Map(m_handle.Get()); }
    void Unmap() override { return Dx11BufferImpl::Unmap(m_handle.Get()); }
    void SetData(const void* data, size_t offset = 0, size_t size = 0) { Dx11BufferImpl::SetData(m_handle.Get(), data, offset, size); }
    
    uint32_t GetSize() const override { return Dx11BufferImpl::GetSize(); }
    ID3D11Buffer* GetNativeObjectPtr() const { return m_handle.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_handle = nullptr;
};

}
