#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

struct ID3D11Buffer;
struct D3D11_BUFFER_DESC;

// Can't put Microsoft::WRL::ComPtr here in header, so actual buffer ptrs are defined in concrete dx11 buffers

namespace pn {

class Dx11BufferImpl
{
public:
    Dx11BufferImpl(BufferMapAccess access, uint32_t size);
    virtual ~Dx11BufferImpl() = default;

    void Init(ID3D11Buffer** handlePtr, const D3D11_BUFFER_DESC& desc, const void* data);

    void* Map(ID3D11Buffer* handle);
    void Unmap(ID3D11Buffer* handle);

    uint32_t GetSize() const { return m_size; }

protected:
    uint32_t m_size;
    BufferMapAccess m_mapAccess;
};

}
