#pragma once

#include <cstdint>

namespace pn {

enum class BufferMapAccess : uint32_t
{
    NoAccess = 0,
    ReadOnly,
    WriteOnly,
    ReadWrite,
};

}