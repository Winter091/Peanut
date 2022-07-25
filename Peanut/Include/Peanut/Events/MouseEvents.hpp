#pragma once

#include <Peanut/Events/Event.hpp>
#include <Peanut/Core/MouseCodes.hpp>

namespace pn {

class MouseButtonPressedEvent : public Event
{
public:
    explicit MouseButtonPressedEvent(pn::MouseButtonCode code) : m_mouseButtonCode(code) {};
    ~MouseButtonPressedEvent() override = default;

    Event* Copy() const override { return new MouseButtonPressedEvent(*this); }

    std::string ToString() const override
    {
        return std::string("MouseButtonPressedEvent: ") + 
            std::to_string(static_cast<uint32_t>(m_mouseButtonCode));
    }

    PN_DEFINE_EVENT_TYPE(MouseButtonPressed);

    pn::MouseButtonCode GetMouseButtonCode() const { return m_mouseButtonCode; }

private:
    pn::MouseButtonCode m_mouseButtonCode;
};


class MouseButtonReleasedEvent : public Event
{
public:
    explicit MouseButtonReleasedEvent(pn::MouseButtonCode code) : m_mouseButtonCode(code) {};
    ~MouseButtonReleasedEvent() override = default;

    Event* Copy() const override { return new MouseButtonReleasedEvent(*this); }

    std::string ToString() const override
    {
        return std::string("MouseButtonReleasedEvent: ") + 
            std::to_string(static_cast<uint32_t>(m_mouseButtonCode));
    }

    PN_DEFINE_EVENT_TYPE(MouseButtonReleased);

    pn::MouseButtonCode GetMouseButtonCode() const { return m_mouseButtonCode; }

private:
    pn::MouseButtonCode m_mouseButtonCode;
};


class MousePosChangedEvent : public Event
{
public:
    MousePosChangedEvent(float x, float y) : m_x(x), m_y(y) {}
    ~MousePosChangedEvent() override = default;

    Event* Copy() const override { return new MousePosChangedEvent(*this); }

    std::string ToString() const override
    {
        return std::string("MousePosChangedEvent: ") + 
            std::to_string(m_x) + ", " + std::to_string(m_y);
    }

    PN_DEFINE_EVENT_TYPE(MousePosChanged);

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }

private:
    float m_x;
    float m_y;
};


class MouseScrolledEvent : public Event
{
public:
    explicit MouseScrolledEvent(float offset) : m_offset(offset) {};
    ~MouseScrolledEvent() override = default;

    Event* Copy() const override { return new MouseScrolledEvent(*this); }

    std::string ToString() const override
    {
        return std::string("MouseScrolledEvent: ") + std::to_string(m_offset);
    }

    PN_DEFINE_EVENT_TYPE(MouseScrolled);

    float GetOffset() const { return m_offset; }

private:
    float m_offset;
};

}
