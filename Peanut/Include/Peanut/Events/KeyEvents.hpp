#pragma once

#include <Peanut/Events/Event.hpp>
#include <Peanut/Core/KeyCodes.hpp>
#include <sstream>

namespace pn {

class KeyPressedEvent : public Event
{
public:
    explicit KeyPressedEvent(pn::KeyCode code) : m_keyCode(code) {};
    ~KeyPressedEvent() override = default;

    Event* Copy() const override { return new KeyPressedEvent(*this); }

    std::string ToString() const override
    {
        return std::string("KeyPressedEvent: ") + std::to_string(static_cast<uint32_t>(m_keyCode));
    }

    PN_DEFINE_EVENT_TYPE(KeyPressed);

    pn::KeyCode GetKeyCode() const { return m_keyCode; }

private:
    pn::KeyCode m_keyCode;
};


class KeyRepeatedEvent : public Event
{
public:
    explicit KeyRepeatedEvent(pn::KeyCode code) : m_keyCode(code) {};
    ~KeyRepeatedEvent() override = default;

    Event* Copy() const override { return new KeyRepeatedEvent(*this); }

    std::string ToString() const override
    {
        return std::string("KeyRepeatedEvent: ") + std::to_string(static_cast<uint32_t>(m_keyCode));
    }

    PN_DEFINE_EVENT_TYPE(KeyRepeated);

    pn::KeyCode GetKeyCode() const { return m_keyCode; }

private:
    pn::KeyCode m_keyCode;
};


class KeyReleasedEvent : public Event
{
public:
    explicit KeyReleasedEvent(pn::KeyCode code) : m_keyCode(code) {};
    ~KeyReleasedEvent() override = default;

    Event* Copy() const override { return new KeyReleasedEvent(*this); }

    std::string ToString() const override
    {
        return std::string("KeyReleasedEvent: ") + std::to_string(static_cast<uint32_t>(m_keyCode));
    }

    PN_DEFINE_EVENT_TYPE(KeyReleased);

    pn::KeyCode GetKeyCode() const { return m_keyCode; }

private:
    pn::KeyCode m_keyCode;
};

}
