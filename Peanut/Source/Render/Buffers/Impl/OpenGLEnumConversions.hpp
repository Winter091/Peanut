#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>
#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Buffers/BufferLayout.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>

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

    inline uint32_t BufferLayoutElementTypeToGlType(BufferLayoutElementType type)
    {
        switch (type) {
            case BufferLayoutElementType::Int8:     return GL_BYTE;
            case BufferLayoutElementType::Int16:    return GL_SHORT;
            case BufferLayoutElementType::Int32:    return GL_INT;
            case BufferLayoutElementType::Uint8:    return GL_UNSIGNED_BYTE;
            case BufferLayoutElementType::Uint16:   return GL_UNSIGNED_SHORT;
            case BufferLayoutElementType::Uint32:   return GL_UNSIGNED_INT;
            case BufferLayoutElementType::Float:    return GL_FLOAT;
            default:                                break;
        }

        PN_CORE_ASSERT(false, "Unknown BufferLayoutElementType enum value: {}", static_cast<uint32_t>(type));
        return 0u;
    }

    inline uint32_t IsBufferLayoutElementTypeInt(BufferLayoutElementType type)
    {
        switch (type) {
            case BufferLayoutElementType::Float:
                return false;
            default:
                break;
        }
        return true;
    }

    inline uint32_t GetIndexBufferDataFormatSize(IndexBufferDataFormat format)
    {
        switch (format) {
            case IndexBufferDataFormat::Uint16: return 2u;
            case IndexBufferDataFormat::Uint32: return 4u;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown IndexBufferDataFormat enum value: {}", static_cast<uint32_t>(format));
        return 0u;
    }

}
