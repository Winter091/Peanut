#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>
#include <Peanut/Core/Assert.hpp>
#include <cstdint>
#include <d3d11.h>

namespace pn {

    inline D3D11_USAGE BufferMapAccessToDxCpuUsageFlags(BufferMapAccess access)
    {
        switch (access) {
        case BufferMapAccess::NoAccess:     return D3D11_USAGE_IMMUTABLE;
        case BufferMapAccess::Read:         return D3D11_USAGE_STAGING;
        case BufferMapAccess::Write:        return D3D11_USAGE_DYNAMIC;
        case BufferMapAccess::WriteDiscard: return D3D11_USAGE_DYNAMIC;
        case BufferMapAccess::ReadWrite:    return D3D11_USAGE_STAGING;
        default: break;
        }

        PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
        return D3D11_USAGE_DEFAULT;
    }

    inline uint32_t BufferMapAccessToDxCpuAccessFlags(BufferMapAccess access)
    {
        switch (access) {
        case BufferMapAccess::NoAccess:     return 0u;
        case BufferMapAccess::Read:         return D3D11_CPU_ACCESS_READ;
        case BufferMapAccess::Write:        return D3D11_CPU_ACCESS_WRITE;
        case BufferMapAccess::WriteDiscard: return D3D11_CPU_ACCESS_WRITE;
        case BufferMapAccess::ReadWrite:    return D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
        default: break;
        }

        PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
        return 0;
    }

}

