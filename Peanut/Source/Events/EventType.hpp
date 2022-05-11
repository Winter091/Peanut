#ifndef __EVENTTYPE_H__
#define __EVENTTYPE_H__

enum class EventType
{
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MousePosChanged,
    MouseScrolled,
    FramebufferSizeChanged,
};

#endif