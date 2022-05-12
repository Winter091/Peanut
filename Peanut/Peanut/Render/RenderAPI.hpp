#ifndef __RENDERPROVIDER_H__
#define __RENDERPROVIDER_H__

#include <cstdint>

namespace pn {

enum class RenderAPI : uint32_t
{
    None = 0,
    OpenGL,
};

}

#endif