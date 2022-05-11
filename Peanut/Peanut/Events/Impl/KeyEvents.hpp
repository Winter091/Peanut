#ifndef __KEYPRESSEDEVENTS_H__
#define __KEYPRESSEDEVENTS_H__

#include <Peanut/Events/Event.hpp>
#include <Peanut/Core/KeyCodes.hpp>

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


class KeyRepeatedEvent : public Event
{
public:
    KeyRepeatedEvent(pn::KeyCode code) : m_keyCode(code) {};
    ~KeyRepeatedEvent() override = default;

    PN_DEFINE_EVENT_TYPE(KeyRepeated);

    pn::KeyCode GetKeyCode() const { return m_keyCode; }

private:
    pn::KeyCode m_keyCode;
};


class KeyReleasedEvent : public Event
{
public:
    KeyReleasedEvent(pn::KeyCode code) : m_keyCode(code) {};
    ~KeyReleasedEvent() override = default;

    PN_DEFINE_EVENT_TYPE(KeyReleased);

    pn::KeyCode GetKeyCode() const { return m_keyCode; }

private:
    pn::KeyCode m_keyCode;
};

}

#endif