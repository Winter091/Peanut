#pragma once

#include <cstdint>

namespace pn {

class Buffer
{
public:
    virtual ~Buffer() = default;

    virtual void* Map() = 0;
    virtual void Unmap() = 0;
    virtual void SetData(const void* data, size_t offset = 0, size_t size = 0) = 0;

    virtual uint32_t GetSize() const = 0;
};

}
