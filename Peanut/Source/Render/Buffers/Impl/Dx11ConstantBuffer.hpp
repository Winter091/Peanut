#pragma once

#include <Peanut/Render/Buffers/ConstantBuffer.hpp>

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

class Dx11ConstantBuffer final : public ConstantBuffer
{
public:
    Dx11ConstantBuffer(BufferMapAccess access, uint32_t size, const void* data);
    ~Dx11ConstantBuffer() override = default;

    void* Map() override;
    void Unmap() override;
    
    uint32_t GetSize() const override { return m_size; }
    ID3D11Buffer* GetNativeObjectPtr() const { return m_handle.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_handle = nullptr;
    uint32_t m_size;
    BufferMapAccess m_mapAccess;
};

}
