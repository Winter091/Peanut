#pragma once

#include <cstdint>

namespace pn {

enum class BufferMapAccess : uint32_t
{
    NoAccess = 0,
    Read,
    Write,
    WriteDiscard,
    ReadWrite,
};

inline bool DoesBufferMapAccessAllowWriting(BufferMapAccess access)
{
    switch (access) {
        case BufferMapAccess::NoAccess:
        case BufferMapAccess::Read:
            return false;
        default:
            return true;
    }
}

}