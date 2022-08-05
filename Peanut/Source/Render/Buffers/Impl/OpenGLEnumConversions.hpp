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
        case BufferMapAccess::Read:         return GL_MAP_READ_BIT;
        case BufferMapAccess::Write:        return GL_MAP_WRITE_BIT;
        case BufferMapAccess::WriteDiscard: return GL_MAP_WRITE_BIT;
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
        case BufferMapAccess::Read:         return GL_MAP_READ_BIT;
        case BufferMapAccess::Write:        return GL_MAP_WRITE_BIT;
        case BufferMapAccess::WriteDiscard: return GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
        case BufferMapAccess::ReadWrite:    return GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
    return 0;
}

}

