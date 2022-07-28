#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>
#include <Peanut/Core/Assert.hpp>
#include <cstdint>
#include <glad/glad.h>

namespace pn {

inline uint32_t BufferMapAccessToGlStorageAccess(BufferMapAccess access)
{
    switch (access) {
        case BufferMapAccess::NoAccess:     return 0u;
        case BufferMapAccess::ReadOnly:     return GL_MAP_READ_BIT;
        case BufferMapAccess::WriteOnly:    return GL_MAP_WRITE_BIT;
        case BufferMapAccess::ReadWrite:    return GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
    return 0;
}

inline uint32_t BufferMapAccessToGlMapAccess(BufferMapAccess access)
{
    PN_CORE_ASSERT(access != BufferMapAccess::NoAccess, "Buffer has no map access, can't get value");

    switch (access) {
        case BufferMapAccess::ReadOnly:     return GL_READ_ONLY;
        case BufferMapAccess::WriteOnly:    return GL_WRITE_ONLY;
        case BufferMapAccess::ReadWrite:    return GL_READ_WRITE;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
    return 0;
}

}

