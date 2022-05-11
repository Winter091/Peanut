#ifndef __EVENTTYPE_H__
#define __EVENTTYPE_H__

#include <cstdint>

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
    FramebufferSizeChanged,
};

#endif