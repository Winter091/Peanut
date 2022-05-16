#ifndef __RENDERAPI_H__
#define __RENDERAPI_H__

#include <cstdint>

namespace pn {

enum class RenderAPI : uint32_t
{
    None = 0,
    OpenGL,
    DirectX11,
};

}

#endif