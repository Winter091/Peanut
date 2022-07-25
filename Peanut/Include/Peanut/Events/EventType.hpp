#pragma once

#include <cstdint>

namespace pn {

enum class EventType : uint32_t
{
    KeyPressed = 0,
    KeyRepeated,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MousePosChanged,
    MouseScrolled,
    WindowSizeChanged,
};

}
