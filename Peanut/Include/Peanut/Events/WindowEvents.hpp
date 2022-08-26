#pragma once

#include <Peanut/Events/Event.hpp>

namespace pn {

    class WindowSizeChangedEvent : public Event
    {
    public:
        WindowSizeChangedEvent(int width, int height) : m_width(width), m_height(height) {}
        ~WindowSizeChangedEvent() override = default;

        Event* Copy() const override { return new WindowSizeChangedEvent(*this); }

        std::string ToString() const override
        {
            return std::string("WindowSizeChangedEvent: ") + 
                std::to_string(m_width) + ", " + std::to_string(m_height);
        }

        PN_DEFINE_EVENT_TYPE(WindowSizeChanged);

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        int m_width;
        int m_height;
    };

}
