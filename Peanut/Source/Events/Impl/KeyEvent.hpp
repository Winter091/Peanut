#ifndef __KEYPRESSEDEVENT_H__
#define __KEYPRESSEDEVENT_H__

#include <Events/Event.hpp>
#include <Core/KeyCodes.hpp>

namespace pn {

class KeyPressedEvent : public Event
{
public:
    KeyPressedEvent(KeyCode code) : m_keyCode(type), m_action(action) {};
    ~KeyPressedEvent() override = default;

    EventType GetEventType() override const { return EventType::KeyPressed; }
    KeyCode GetKeyCode() const { return m_keyCode; }

private:
    KeyCode m_keyCode;
};

}

#endif