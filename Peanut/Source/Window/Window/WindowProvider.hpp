#ifndef __WINDOWPROVIDER_H__
#define __WINDOWPROVIDER_H__

#include <cstdint>

namespace pn {

enum class WindowProvider : uint32_t
{
    None = 0,
    GLFW,
};

}

#endif