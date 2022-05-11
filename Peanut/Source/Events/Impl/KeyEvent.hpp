#ifndef __KEYPRESSEDEVENT_H__
#define __KEYPRESSEDEVENT_H__

#include <Events/Event.hpp>
#include <Core/KeyCodes.hpp>

namespace pn {

class KeyPressedEvent : public Event
{
public:
    KeyPressedEvent(pn::KeyCode code) : m_keyCode(code) {};
    ~KeyPressedEvent() override = default;

    PN_DEFINE_EVENT_TYPE(KeyPressed);

    pn::KeyCode GetKeyCode() const { return m_keyCode; }

private:
    pn::KeyCode m_keyCode;
};

}

#endif