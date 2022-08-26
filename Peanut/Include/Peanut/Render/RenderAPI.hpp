#pragma once

#include <cstdint>

namespace pn {

    enum class RenderAPI : uint32_t
    {
        None = 0,
        OpenGL,
        Dx11,
    };

}
