#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

struct ID3D11Buffer;
struct D3D11_BUFFER_DESC;

namespace pn {

class Dx11BufferImpl
{
public:
    Dx11BufferImpl(BufferMapAccess access, uint32_t size);
    virtual ~Dx11BufferImpl();

    void Init(const D3D11_BUFFER_DESC& desc, const void* data);

    void* Map();
    void Unmap();
    void SetData(const void* data, size_t offset, size_t size);

    uint32_t GetSize() const { return m_size; }
    ID3D11Buffer* GetNativeObjectPtr() const { return m_handle; }

protected:
    ID3D11Buffer* m_handle = nullptr;
    uint32_t m_size;
    BufferMapAccess m_mapAccess;
};

}
