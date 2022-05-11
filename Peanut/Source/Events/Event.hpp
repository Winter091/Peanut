#ifndef __EVENT_H__
#define __EVENT_H__

#include <Events/EventType.hpp>

class Event
{
public:
    virtual EventType GetEventType() const = 0;

    virtual ~Event() = default;
};

#endif