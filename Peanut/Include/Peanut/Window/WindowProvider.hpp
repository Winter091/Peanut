#pragma once

#include <cstdint>

namespace pn {

    enum class WindowProvider : uint32_t
    {
        None = 0,
        GLFW,
    };

}
